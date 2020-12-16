#include "codegen.h"

int currentLevel = -1, currentDispl = 0, globalDisplCounter = 0;
int currentEnfn = 0;
SymbEntryPtr SymbTable = NULL;
bool load = false;

TypeDescrPtr getType(TreeNodePtr p) {
  if(!strcmp(p->str, "integer")) {
    return newTypeDescr(T_PREDEF, T_INT, 1);
  }else if(!strcmp(p->str, "boolean")) {
    return newTypeDescr(T_PREDEF, T_BOOL, 1);
  }else {
    SymbEntryPtr ste = searchSte(p->str);
    ste->descr->type->constr = T_ARRAY;
    return ste->descr->type;
  }
}

int getIndx(TreeNodePtr p) {
  //if(!strcmp(p->str, "integer")) {
  //  return newTypeDescr(T_PREDEF, T_INT, 1);
  //}else if(!strcmp(p->str, "boolean")) {
  //  return newTypeDescr(T_PREDEF, T_BOOL, 1);
  //}else {
  //  SymbEntryPtr ste = searchSte(p->str);
  //  ste->descr->type->constr = T_ARRAY;
  //  return ste->descr->type;
  //}
  return 1;
}

int newLabel() {
  static int label = 0;
  label++;
  return label;
}

TypeDescrPtr getOp(TreeNodePtr p) {
  if(!strcmp(p->str, "&&")) {
    genCode0("LAND");
    return predefBool();
  }else if(!strcmp(p->str, "+")) {
    genCode0("ADDD");
    return predefInt();
  }else if(!strcmp(p->str, "*")) {
    genCode0("MULT");
    return predefInt();
  }else if(!strcmp(p->str, "/")) {
    genCode0("DIVI");
    return predefInt();
  }else if(!strcmp(p->str, "-")) {
    genCode0("SUBT");
    return predefInt();
  }else if(!strcmp(p->str, "<")) {
    genCode0("LESS");
    return predefBool();
  }else if(!strcmp(p->str, "<=")) {
    genCode0("LEQU");
    return predefBool();
  }else if(!strcmp(p->str, ">")) {
    genCode0("GRTR");
    return predefBool();
  }else if(!strcmp(p->str, "!")) {
    genCode0("LNOT");
    return predefBool();
  }
  printf("ERROR %s\n", p->str);
  exit(1);
}

void genCode0(char *cmd) {
  printf("      %s\n", cmd);
}

void genCode1(char *cmd, int counter) {
  printf("      %s   %i\n", cmd, counter);
}

void genCode2(char *cmd, char *expr) {
  printf("      %s   %s\n", cmd, expr);
}

void genCode3(char *cmd, int a, int b) {
  printf("      %s   %i,%i\n", cmd, a, b);
}

void genCode4(char *cmd, int a, int b) {
  printf("      %s   L%i,%i\n", cmd, a, b);
}

void genCodeLabels(int label, int a, int b) {
  printf("L%i:   ENLB   %i,%i\n", label, a, b);
}

void genCodeEnfn(int label, char *cmd, int enfn) {
  printf("L%i:   %s   %i\n", label, cmd, enfn);
}

void genCodeLabel(int label, char *cmd) {
  printf("L%i:   %s\n", label, cmd);
}

void genJump(int label) {
  printf("      JUMP   L%i\n", label);
}

char* getIdent(TreeNodePtr p) {
  return p->str;
}

void genCodeJump(char *cmd, int label) {
  printf("      %s   L%i\n", cmd, label);
}

TypeDescrPtr predefBool() {
  return newTypeDescr(T_PREDEF, T_BOOL, 1);
}

TypeDescrPtr predefInt() {
  return newTypeDescr(T_PREDEF, T_INT, 1);
}

void processIteration(TreeNodePtr p) {
  int label1 = newLabel();
  int label2 = newLabel();
  genCodeLabel(label1, "NOOP");
  TypeDescrPtr t = processExpr(p->comps[0]);
  if(!compatibleTypes(t, predefBool()))
    SemanticError();
  genCodeJump("JMPF", label2);
  processStatements(p->comps[1]);
  genCodeJump("JUMP", label1);
  genCodeLabel(label2, "NOOP");
}

void processCondition(TreeNodePtr p) {
  int label1 = newLabel();
  int label2 = newLabel();
  load = true;
  TypeDescrPtr t = processExpr(p->comps[0]);
  load = false;
  if(!compatibleTypes(t, predefBool()))
    SemanticError();
  genCodeJump("JMPF", label1);
  processStatements(p->comps[1]);
  genCodeJump("JUMP", label2);
  genCodeLabel(label1, "NOOP");
  processStatements(p->comps[2]);
  genCodeLabel(label2, "NOOP");
}

void processVariables(TreeNodePtr p) {
  p = invertList(p);
  for( ; (p!=NULL); p=p->next)
    processVarDecl(p);
}

void processVarDecl(TreeNodePtr p) {
  TreeNodePtr pvars = invertList(p->comps[0]);
  SymbEntryPtr ste; char *id;
  int size = 0, element = 1;

  TreeNodePtr psize = p->comps[2];
  if(p->comps[2]) {
    element = atoi(psize->str);
    for ( ; (psize!=NULL); psize=psize->next ) {
      if(!size)
        size = atoi(psize->str);
      else
        size = size*atoi(psize->str);
    }
  }

  TypeDescrPtr ptype = getType(p->comps[1]);
  if(size) {
    ptype->size = size;
    ptype->constr = T_ARRAY;
  }

  for ( ; (pvars!=NULL); pvars=pvars->next ) {
    id = getIdent(pvars);
    ste = newSymbEntry(S_VARIABLE, id);
    ste->level = currentLevel;
    ste->descr->displ = currentDispl;
    ste->descr->type = ptype;
    insertSymbolTable(ste);
    currentDispl += ptype->size;
    globalDisplCounter += ptype->size;
  }
}

TypeDescrPtr processExpr(TreeNodePtr p) {
  TypeDescrPtr t;
  switch(p->categ) {
    case C_INT:
      return processInt(p);
    case C_VAR:
      return processVar(p);
    case C_UN_EXPR:
      return processUnExpr(p);
    case C_BIN_EXPR:
      return processBinExpr(p);
    case C_EXPR:
      if(!p->comps[1]) {
        return processExpr(p->comps[0]);
      }else {
        load = true;
        processExpr(p->comps[0]);
        t = processExpr(p->comps[1]);
        load = false;
        return t;
      }
    case C_REL_EXPR:
      return processRelational(p);
    case C_IDENT:
      return processIdent(p);
    case C_FUNCTION_CALL:
      processFunctionCall(p);
      return predefInt();
  }
  return predefInt();
}

TypeDescrPtr processIdent(TreeNodePtr p) {
  SymbEntryPtr ste = searchSte(p->str);
  return predefBool();
}

TypeDescrPtr processRelational(TreeNodePtr p) {
  TypeDescrPtr t = processExpr(p->comps[1]);
  TypeDescrPtr op = getOp(p->comps[0]);
  //if (!compatibleTypesBinOp(op,t0,t1))
  //  SemanticError();
  return predefBool();
}

TypeDescrPtr processInt(TreeNodePtr p) {
  TypeDescrPtr type = newTypeDescr(T_PREDEF, T_INT, 1);
  genCode2("LDCT", p->str);
  return type;
}

TypeDescrPtr processBool(TreeNodePtr p) {
  TypeDescrPtr type = newTypeDescr(T_PREDEF, T_BOOL, 1);
  return type;
}

TypeDescrPtr processArray(TreeNodePtr p, SymbEntryPtr ste) {
  if(p) {
    genCode3("LADR", ste->level, ste->descr->displ);
    TreeNodePtr indexs = invertList(p);
    for( ; (indexs!=NULL); indexs=indexs->next) {
      load = true;
      TreeNodePtr pindex = indexs->comps[0];
      TypeDescrPtr texpr = processExpr(pindex);
      load = false;
      genCode1("INDX", texpr->size);
    }
    if(ste->descr->type->size == 1)
      genCode0("CONT");
    else
      genCode1("LDMV", ste->descr->type->size);
  }else {
    genCode3("LADR", ste->level, ste->descr->displ);
    genCode1("LDMV", ste->descr->type->size);
  }

  return ste->descr->type;
}

TypeDescrPtr processVar(TreeNodePtr p) {
  char *id = getIdent(p->comps[0]);

  if(!strcmp(id, "true")) {
    genCode2("LDCT", "1");
    return predefBool();
  }

  if(!strcmp(id, "false")) {
    genCode2("LDCT", "0");
    return predefBool();
  }

  SymbEntryPtr ste = searchSte(id);

  if(ste->descr->type->constr == T_ARRAY) {
    return processArray(p->comps[1], ste);
  }else {
    if(load)
      genCode3("LDVL", ste->level, ste->descr->displ);
    else
      genCode3("STVL", ste->level, ste->descr->displ);
    return ste->descr->type;
  }

}

TypeDescrPtr processUnExpr(TreeNodePtr p) {
  TypeDescrPtr t0, t1, t2;
  t0 = processExpr(p->comps[0]);
  t1 = processExpr(p->comps[1]);

  if(p->comps[2])
    t2 = processExpr(p->comps[2]);

  if(p->comps[0]->str) {
    if(!strcmp(p->comps[0]->str, "-"))
      genCode0("NEGT");
  }

  if(p->comps[1]->str) {
    if(!strcmp(p->comps[1]->str, "+"))
      genCode0("ADDD");
  }

  if(p->comps[0]->str) {
    if(!strcmp(p->comps[0]->str, "!")) {
      genCode0("LNOT");
      return predefBool();
    }
  }
  //if (!compatibleTypesUnOp(op,t))
  //  SemanticError();
  return predefInt();
}

TypeDescrPtr processBinExpr(TreeNodePtr p) {
  if(p) {
    TypeDescrPtr t0 = processExpr(p->comps[0]);
    TypeDescrPtr t1 = processExpr(p->comps[2]);
    TypeDescrPtr op = getOp(p->comps[1]);
    if (!compatibleTypesBinOp(op, t0, t1))
      SemanticError();
    return op;
  }
}

void processAssign(TreeNodePtr p) {
  TypeDescrPtr tvar, texpr;
  TreeNodePtr pvar = p->comps[0]->comps[0];
  TreeNodePtr pexpr = p->comps[1];
  TreeNodePtr indexs;
  char *id = pvar->str;
  int size = 1;

  SymbEntryPtr ste = searchSte(id);

  if(ste == NULL)
    SemanticError(); // undefined variable

  if((ste->categ != S_VARIABLE) && (ste->categ != S_PARAMETER))
    SemanticError();

  if(!compatibleTypes(tvar, texpr))
    SemanticError(); // incompatible types

  switch(ste->descr->type->constr) {
    case T_FUNCTION:
      break;
    case T_ARRAY:
      load = true;
      genCode3("LADR", ste->level, ste->descr->displ);
      indexs = invertList(p->comps[0]->comps[1]);
      for( ; (indexs!=NULL); indexs=indexs->next) {
        load = true;
        TreeNodePtr pindex = indexs->comps[0];
        TypeDescrPtr texpr = processExpr(pindex);
        load = false;
        genCode1("INDX", texpr->size);
      }
      //pindexB = p->comps[0]->comps[2];
      //if(pindexA) {
      //  texpr = processExpr(pindexA);
      //  genCode1("INDX", texpr->size);
      //}else {
      size = ste->descr->type->size;
      //}
      //if(pindexB) {
      //  texpr = processExpr(pindexB);
      //  genCode1("INDX", texpr->size);
      //}else {
      //  size = ste->descr->type->size;
      //}
      texpr = processExpr(pexpr);
      genCode1("STMV", size);
      load = false;
      break;
    default:
      load = true;
      texpr = processExpr(pexpr);
      load = false;

      tvar = ste->descr->type;

      if((ste->categ == S_PARAMETER) && (ste->descr->pass == P_VARIABLE))
        genCode3("STVI", ste->level, ste->descr->displ);
      else
        genCode3("STVL", ste->level, ste->descr->displ);
  }
}

void processFunctionCall(TreeNodePtr p) {
  TypeDescrPtr tvar, texpr;
  TreeNodePtr funcall = p->comps[0];
  TreeNodePtr pexpr = invertList(p->comps[1]);

  if(!strcmp(funcall->str, "write")) {
    for ( ; (pexpr!=NULL); pexpr=pexpr->next ) {
      load = true;
      texpr = processExpr(pexpr);
      load = false;
      genCode0("PRNT");
    }
  }else if(!strcmp(funcall->str, "read")) {
    for ( ; (pexpr!=NULL); pexpr=pexpr->next ) {
      genCode0("READ");
      texpr = processExpr(pexpr);
    }
  }else {
    SymbEntryPtr ste = searchSte(funcall->str);
    if(ste->descr->type)
      genCode1("ALOC", 1);
    TypeDescrPtr ptype = NULL;
    TypeDescrPtr buffer = NULL;
    for ( ; (pexpr!=NULL); pexpr=pexpr->next ) {
      load = true;
      if(!ptype) {
        ptype = processExpr(pexpr);
      }else {
        buffer = processExpr(pexpr);
        buffer->next = ptype;
        ptype = buffer;
      }
      load = false;
    }
    ptype = invertTypeList(ptype);
    if(!compatibleTypesFunctionCall(ste->descr->params, ptype))
      SemanticError();
    genCode4("CFUN", ste->descr->entLabel, ste->level-1);
  }
}

void processFunctions(TreeNodePtr p) {
  currentEnfn++;
  TreeNodePtr pvars = invertList(p);
  for ( ; (pvars!=NULL); pvars=pvars->next ) {
    processFuncDecl(pvars, false);
  }
  currentEnfn--;
}

void processLabels(TreeNodePtr p) {
  TreeNodePtr pvars = invertList(p->comps[0]);
  for ( ; (pvars!=NULL); pvars=pvars->next ) {
    char *ident = getIdent(pvars);
    int id = newLabel();
    SymbEntryPtr ste;
    ste = newSymbEntry(S_LABEL, ident);
    ste->level = id;
    ste->descr->displ = currentDispl;
    ste->descr->type = NULL;
    insertSymbolTable(ste);
  }
}

void processTypes(TreeNodePtr p) {
  TreeNodePtr pvars = invertList(p);

  for ( ; (pvars!=NULL); pvars=pvars->next ) {
    char *ident = getIdent(pvars->comps[0]);
    SymbEntryPtr ste = newSymbEntry(S_TYPE, ident);
    ste->descr->displ = currentDispl;

    TypeDescrPtr ptype = getType(pvars->comps[1]);
    TreeNodePtr psize = pvars->comps[2];
    int size = 0;

    if(pvars) {
      ptype->constr = T_ARRAY;
      for ( ; (psize!=NULL); psize=psize->next ) {
        if(!size)
          size = atoi(psize->str);
        else
          size = size*atoi(psize->str);
      }
    }
    ptype->size = size;
    ste->descr->type = ptype;

    insertSymbolTable(ste);
  }
}

void processGoto(TreeNodePtr p) {
  SymbEntryPtr ste = searchSte(p->comps[0]->str);
  genJump(ste->level);
}

void processLabel(TreeNodePtr p) {
  char *ident = getIdent(p->comps[0]);
  SymbEntryPtr ste = searchSte(ident);
  genCodeLabels(ste->level, 0, currentDispl);
  processStatements(p->comps[1]);
}

void processStatements(TreeNodePtr p) {
  if(p) {
    TreeNodePtr pvars = invertList(p);
    for ( ; (pvars!=NULL); pvars=pvars->next ) {
      switch(pvars->categ) {
        case C_ASSIGN:
          processAssign(pvars);
          break;
        case C_FUNCTION_CALL:
          processFunctionCall(pvars);
          break;
        case C_WHILE:
          processIteration(pvars);
          break;
        case C_IF:
          processCondition(pvars);
          break;
        case C_GOTO:
          processGoto(pvars);
          break;
        case C_LABEL:
          processLabel(pvars);
          break;
        case C_RETURN:
          processReturn(pvars);
          break;
        default:
          puts("NO ONE");
      }
    }
  }
}

void processReturn(TreeNodePtr p) {
  if(p->comps[0]) {
    load = true;
    processExpr(p->comps[0]);
    load = false;
    SymbEntryPtr ste = searchLastFunction();
    genCode3("STVL", ste->level, ste->descr->displ);
  }
}

TypeDescrPtr processFormals(TreeNodePtr p, int *lastDispl) {
  TreeNodePtr t = p->comps[2];
  TypeDescrPtr types = NULL, buffer;
  for ( ; (t!=NULL); t=t->next ) {
    TreeNodePtr p = t;
    TreeNodePtr pvars = p->comps[0];
    for ( ; (pvars!=NULL); pvars=pvars->next ) {
      char *id = getIdent(pvars);
      SymbEntryPtr ste = newSymbEntry(S_PARAMETER, id);
      TypeDescrPtr ptype = getType(p->comps[1]);
      ste->level = currentLevel;
      (*lastDispl)--;
      ste->descr->displ = *lastDispl;
      ste->descr->type = ptype;

      if(!types) {
        types = ptype;
      }else {
        buffer = ptype;
        buffer->next = types;
        types = buffer;
      }

      insertSymbolTable(ste);
    }
  }

  return types;
}

void processFuncDecl(TreeNodePtr p, bool ismain) {
  char *fname = getIdent(p->comps[1]);
  TypeDescrPtr resType = NULL, funType;
  int lastDispl = -4, entLabel, retLabel, midLabel;
  SymbEntryPtr func;
  TypeDescrPtr formals;

  if(p->comps[3]->comps[0])
    processLabels(p->comps[3]);

  if(p->comps[3]->comps[3] || !ismain)
    entLabel = newLabel();

  if(!ismain)
    retLabel = newLabel();

  currentLevel++;
  if (p->comps[0])
    resType = getType(p->comps[0]);

  formals = processFormals(p, &lastDispl);

  if (resType) {
    lastDispl -= resType->size;
  }

  func = newSymbEntry(S_FUNCTION, fname);
  func->descr->result = resType;
  func->descr->params = formals;
  func->level = currentLevel;
  func->descr->displ = lastDispl;
  func->descr->type = resType;
  func->descr->entLabel = entLabel;
  insertSymbolTable(func);

  if (resType) {
    lastDispl += resType->size;
  }

  if(ismain) {
    genCode0("MAIN");
  }else {
    genCodeEnfn(entLabel, "ENFN", currentEnfn);
  }

  if(p->comps[3]->comps[1])
    processTypes(p->comps[3]->comps[1]);

  processVariables(p->comps[3]->comps[2]);

  if(ismain && globalDisplCounter)
    genCode1("ALOC", globalDisplCounter);

  if(p->comps[3]->comps[3]) {
    if(!ismain) {
      midLabel = newLabel();
      genCodeJump("JUMP", midLabel);
    }else {
      genCodeJump("JUMP", entLabel);
    }
    processFunctions(p->comps[3]->comps[3]);
    if(!ismain)
      genCodeLabel(midLabel, "NOOP");
    else
      genCodeLabel(entLabel, "NOOP");
  }

  processStatements(p->comps[3]->comps[4]);

  if(!ismain)
    genCodeJump("JUMP", retLabel);

  if(currentDispl && ismain)
    genCode1("DLOC", currentDispl);

  if(ismain) {
    genCode0("STOP\n      END");
  }else {
    genCodeLabel(retLabel, "NOOP");
    genCode1("RTRN", -lastDispl-4);
  }

  currentLevel--;
  if(currentLevel != -1)
    restoreSymbTable();
}

TreeNodePtr invertList(TreeNodePtr p) {
  TreeNodePtr pvars = NULL, r;
  while (p != NULL) {
    r = p->next;
    p->next = pvars;
    pvars = p;
    p = r;
  }
  return pvars;
}

TypeDescrPtr invertTypeList(TypeDescrPtr p) {
  TypeDescrPtr ptypes = NULL, r;
  while (p != NULL) {
    r = p->next;
    p->next = ptypes;
    ptypes = p;
    p = r;
  }
  return ptypes;
}

void processProgram(void *p) {
  //dumpTree(p);
  processFuncDecl(p, true);
}
