#include "codegen.h"

int currentLevel = -1, currentDispl = 0, globalDisplCounter = 0;
SymbEntryPtr SymbTable = NULL;

TypeDescrPtr getType(TreeNodePtr p) {
  TypeDescrPtr type = (TypeDescrPtr) malloc(sizeof(TypeDescr));
  type->constr = T_PREDEF;
  type->size = 1;
  return type;
}

char* getIdent(TreeNodePtr p) {
  return p->str;
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

void genCode0(char *cmd) {
  printf("      %s\n", cmd);
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
    //ste->descr.displ = currentDispl;
    //ste->descr.type = ptype;
    insertSymbolTable(ste);
    currentDispl += ptype->size;
    globalDisplCounter += ptype->size;
  }
}

TypeDescrPtr processExpr(TreeNodePtr p) {
  switch(p->categ) {
    case C_INT: genCode2("LDCT", p->str);
  //  case C_IDENT: return processExprIdent(p);
  //  case C_UN_EXPR: return processUnExpr(p);
  //  case C_BIN_EXPR: return processBinExpr(p);
  }
}

void processAssign(TreeNodePtr p) {
  TypeDescrPtr tvar,texpr;
  TreeNodePtr pvar = p->comps[0]->comps[0];
  TreeNodePtr pexpr = p->comps[1];
  char *id = pvar->str;
  SymbEntryPtr ste = searchSte(id);
  if(ste == NULL) printf("erro\n"); // undefined variable
  //if (ste==NULL) SemanticError(); // undefined variable
  //if ((ste->categ!=S_VARIABLE)&&(ste->categ!=S_PARAMETER))
  //  SemanticError();
  //tvar = ste->descr->type;
  texpr = processExpr(pexpr);
  //if (!compatibleTypes(tvar,texpr))
  //  SemanticError(); // incompatible types
  //if ((ste->categ==S_PARAMETER)&&(ste->descr->pass==P_VARIABLE))
  //  genCode2("STVI",ste->level,ste->descr->displ);
  //else
  genCode3("STVL", ste->level, 0);
  //genCode3("STVL", ste->level, ste->descr->displ);
}

void processStatements(TreeNodePtr p) {
  if(p) {
    TreeNodePtr pvars = invertList(p);
    for ( ; (pvars!=NULL); pvars=pvars->next ) {
      switch(pvars->categ) {
        case C_ASSIGN:
          processAssign(pvars);
          break;
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
