#include "codegen.h"

int currentLevel = -1, currentDispl = 0, globalDisplCounter = 0;
SymbEntryPtr SymbTable = NULL;
bool load = false;

TypeDescrPtr getType(TreeNodePtr p) {
  if(!strcmp(p->str, "integer"))
    return newTypeDescr(T_PREDEF, T_INT, 1);
  else
    return newTypeDescr(T_PREDEF, T_BOOL, 1);
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
  //if(counter)
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
  TypeDescrPtr ptype = getType(p->comps[1]);
  SymbEntryPtr ste; char *id;
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
      if(!p->comps[1])
        return processExpr(p->comps[0]);
      else {
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

TypeDescrPtr processVar(TreeNodePtr p) {
  char *id = p->comps[0]->str;
  if(!strcmp(id, "true")) {
    genCode2("LDCT", "1");
    return predefBool();
  }else if(!strcmp(id, "false")) {
    genCode2("LDCT", "0");
    return predefBool();
  }else {
    SymbEntryPtr ste = searchSte(id);
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
  //if (!compatibleTypesUnOp(op,t))
  //  SemanticError();
  return predefInt();
}

TypeDescrPtr processBinExpr(TreeNodePtr p) {
  if(p) {
    TypeDescrPtr t0 = processExpr(p->comps[0]);
    TypeDescrPtr t1 = processExpr(p->comps[2]);
    TypeDescrPtr op = getOp(p->comps[1]);
    //if (!compatibleTypesBinOp(op,t0,t1))
    //  SemanticError();
    return op;
  }
}

void processAssign(TreeNodePtr p) {
  TypeDescrPtr tvar,texpr;
  TreeNodePtr pvar = p->comps[0]->comps[0];
  TreeNodePtr pexpr = p->comps[1];
  char *id = pvar->str;
  SymbEntryPtr ste = searchSte(id);
  if(ste == NULL)
    SemanticError(); // undefined variable
  if((ste->categ != S_VARIABLE) && (ste->categ != S_PARAMETER))
    SemanticError();
  tvar = ste->descr->type;
  load = true;
  texpr = processExpr(pexpr);
  load = false;
  if(!compatibleTypes(tvar, texpr))
    SemanticError(); // incompatible types
  if((ste->categ == S_PARAMETER) && (ste->descr->pass == P_VARIABLE))
    genCode3("STVI", ste->level, ste->descr->displ);
  else
    genCode3("STVL", ste->level, ste->descr->displ);
}

void processFunctionCall(TreeNodePtr p) {
  TypeDescrPtr tvar, texpr;
  TreeNodePtr funcall = p->comps[0];
  TreeNodePtr pexpr = invertList(p->comps[1]);
  if(!strcmp(funcall->str, "write")) {
    for ( ; (pexpr!=NULL); pexpr=pexpr->next ) {
      load = true;
      texpr = processExpr(pexpr);
      genCode0("PRNT");
      load = false;
    }
  }else if(!strcmp(funcall->str, "read")) {
    for ( ; (pexpr!=NULL); pexpr=pexpr->next ) {
      genCode0("READ");
      texpr = processExpr(pexpr);
    }
  }else {
    SymbEntryPtr ste = searchSte(funcall->str);
    genCode4("CFUN", ste->descr->entLabel, ste->descr->displ);
  }
}

void processFunctions(TreeNodePtr p) {
  TreeNodePtr pvars = p;
  for ( ; (pvars!=NULL); pvars=pvars->next ) {
    processFuncDecl(pvars, false);
  }
}

void processLabels(TreeNodePtr p) {
  TreeNodePtr pvars = p;
  for ( ; (pvars!=NULL); pvars=pvars->next ) {
    char *ident = pvars->comps[0]->str;
    int id = newLabel();
    SymbEntryPtr ste;
    ste = newSymbEntry(S_LABEL, ident);
    ste->level = id;
    //ste->descr->displ = currentDispl;
    ste->descr->type = NULL;
    insertSymbolTable(ste);
  }
}

void processGoto(TreeNodePtr p) {
  SymbEntryPtr ste = searchSte(p->comps[0]->str);
  genJump(ste->level);
}

void processLabel(TreeNodePtr p) {
  SymbEntryPtr ste = searchSte(p->comps[0]->str);
  genCode0("ENLB   0,0");
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
      }
    }
  }
}

void processFuncDecl(TreeNodePtr p, bool ismain) {
  char *fname = getIdent(p->comps[1]);
  TypeDescrPtr resType = NULL, funType;
  int lastDispl = -4, entLabel, retLabel;
  SymbEntryPtr formals, func;

  if(p->comps[3]->comps[3] || !ismain) {
    entLabel = newLabel();
  }

  if(!ismain)
    retLabel = newLabel();

  currentLevel++;
  if (p->comps[0])
    resType = getType(p->comps[0]);
  if (resType)
    lastDispl -= resType->size;

  func = newSymbEntry(S_FUNCTION, fname);
  func->descr->result = resType;
  //func->descr->params = formals;
  func->level = currentLevel-1;
  //func->descr->displ = lastDispl;
  func->descr->displ = 0;
  func->descr->entLabel = entLabel;
  insertSymbolTable(func);

  if(ismain) {
    genCode0("MAIN");
  }else {
    genCodeLabel(entLabel, "ENFN   1");
  }

  if(p->comps[3]->comps[0])
    processLabels(p->comps[3]);
  //processTypes(p->comps[4]);
  processVariables(p->comps[3]->comps[2]);
  if(ismain && globalDisplCounter)
    genCode1("ALOC", globalDisplCounter);
  //loadFormalsSymbolTable(formals);
  if(p->comps[3]->comps[3]) {
    genCode0("JUMP   L1");
    processFunctions(p->comps[3]->comps[3]);
    genCodeLabel(entLabel, "NOOP");
  }
  processStatements(p->comps[3]->comps[4]);

  if(currentDispl && ismain)
    genCode1("DLOC", currentDispl);

  if(ismain) {
    genCode0("STOP\n      END");
  }else {
    genCodeLabel(retLabel, "NOOP");
    genCode1("RTRN", -lastDispl-4);
  }
  currentLevel--;
  //restoreSymbTable();
  //if(p->comps[0] != NULL)
  //  resType = getType(p->comps[0]);
}

void processProgram(void *p) {
  //dumpTree(p);
  processFuncDecl(p, true);
}
