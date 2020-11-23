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

char* getOp(TreeNodePtr p) {
  if(!strcmp(p->str, "&&"))
    return "LAND";
  else if(!strcmp(p->str, "+"))
    return "ADDD";
  else if(!strcmp(p->str, "*"))
    return "MULT";
  else if(!strcmp(p->str, "/"))
    return "DIVI";
  else
    return "ERROR";
}

void genCode0(char *cmd) {
  printf("      %s\n", cmd);
}

void genCode1(char *cmd, int counter) {
  if(counter)
    printf("      %s   %i\n", cmd, counter);
}

void genCode2(char *cmd, char *expr) {
  printf("      %s   %s\n", cmd, expr);
}

void genCode3(char *cmd, int a, int b) {
  printf("      %s   %i,%i\n", cmd, a, b);
}

void genCodeLabel(int label) {
  printf("L%i:   NOOP\n", label);
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
  TypeDescrPtr t;
  genCodeLabel(label1);
  t = processExpr(p->comps[0]);
  if(!compatibleTypes(t, predefBool()))
    SemanticError();
  genCodeJump("JMPF", label2);
  processStatements(p->comps[1]);
  genCodeJump("JUMP", label1);
  genCodeLabel(label2);
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
  TreeNodePtr pvars;
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
    case C_REL_EXPR:
      return processExpr(p);
  }
  return predefInt();
}

TypeDescrPtr processInt(TreeNodePtr p) {
  TypeDescrPtr type = newTypeDescr(T_PREDEF, T_INT, 1);
  genCode2("LDCT", p->str);
  return type;
}

TypeDescrPtr processVar(TreeNodePtr p) {
  char *id = p->comps[0]->str;
  SymbEntryPtr ste = searchSte(id);
  if(load)
    genCode3("LDVL", ste->level, ste->descr->displ);
  else
    genCode3("STVL", ste->level, ste->descr->displ);
  return ste->descr->type;
}

TypeDescrPtr processUnExpr(TreeNodePtr p) {
  //char *op = getOp(p->comps[0]);
  //TypeDescrPtr t = processExpr(p->comps[1]);
  //if (!compatibleTypesUnOp(op,t))
  //  SemanticError();
  //genCode0(mepaUnInstr(op));
  //return t;
}

TypeDescrPtr processBinExpr(TreeNodePtr p) {
  if(p) {
  TypeDescrPtr t0 = processExpr(p->comps[0]);
  TypeDescrPtr t1 = processExpr(p->comps[2]);
  char *op = getOp(p->comps[1]);
  //if (!compatibleTypesBinOp(op,t0,t1))
  //  SemanticError();
  //genCode0(mepaBinInstr(op));
  genCode0(op);
  if(relational(op))
    return predefBool();
  else
    return predefInt();
  }
}

bool relational(char *op) {
  if(!strcmp(op, ">")) {
    return true;
  }
  return false;
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
  texpr = processExpr(pexpr);
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
  }
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
          processIteration(p);
          break;
        case C_GOTO:
          puts("goto");
        default:
          printf("%i\n", pvars->categ);
      }
    }
  }
}

void processFuncDecl(TreeNodePtr p, bool ismain) {
  char *fname = getIdent(p->comps[1]);
  TypeDescrPtr resType = NULL, funType;
  int lastDispl = -4, entLabel, retLabel;
  SymbEntryPtr formals, func;

  currentLevel++;
  if(ismain)
    genCode0("MAIN");

  //processLabels(p->comps[3]);
  //processTypes(p->comps[4]);
  processVariables(p->comps[3]->comps[2]);
  genCode1("ALOC", globalDisplCounter);
  //loadFormalsSymbolTable(formals);
  //processFunctions(p->comps[2]);
  processStatements(p->comps[3]->comps[4]);

  //printSymbolTable();

  //genCodeLabel(retLabel);
  genCode1("DLOC", currentDispl);
  if(ismain) {
    genCode0("STOP\n      END");
  }else {
    //genCode1("RTRN",-lastDispl-4);
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
