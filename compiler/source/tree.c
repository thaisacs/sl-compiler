#include "tree.h"

char *C_OPT_SYMBOL[] = {
  [C_SUM]="+",
  [C_MINUS]="-",
  [C_MUL]="*",
  [C_DIV]="/",
  [C_AND]="&&",
  [C_OR]="||",
  [C_NOT]="!",
  [C_LESS_EQUAL]="<=",
  [C_LESS]="<",
  [C_EQUAL]="=",
  [C_DIFFERENT]="!=",
  [C_GREATER_EQUAL]=">+",
  [C_GREATER]=">"
};

int top = 0;
TreeNodePtr stack[MAX_STACK_SIZE];

char* copy_str(char *s) {
  char *s_cpy = malloc (1 + strlen(s));
  strcpy (s_cpy, s);
  return s_cpy;
}

void *getTree() {
  return stack[top-1];
}

void test(TreeNodePtr p, int *functions, int *funcalls,
    int *whiles, int *ifs, int *bin) {
  if(p) {
    if(p->categ == C_FUNCTION)
      (*functions)++;
    else if(p->categ == C_FUNCTION_CALL)
      (*funcalls)++;
    else if(p->categ == C_WHILE)
      (*whiles)++;
    else if(p->categ == C_IF)
      (*ifs)++;
    else if(p->categ == C_BIN_EXPR)
      (*bin)++;

    for(int i = 0; i < p->n; i++) {
      test(p->comps[i], functions, funcalls, whiles, ifs, bin);
    }

    if(p->next) {
      test(p->next, functions, funcalls, whiles, ifs, bin);
    }
  }
}

void counts(void *p, int *functions, int *funcalls,
    int *whiles, int *ifs, int *bin) {

  *functions = 0;
  *funcalls = 0;
  *whiles = 0;
  *ifs = 0;
  *bin = 0;

  test(p, functions, funcalls, whiles, ifs, bin);
}

void genNode3(Categ cat, int n, char *s) {
  TreeNodePtr np = malloc(sizeof(TreeNode));
  np->categ = cat;
  np->next = NULL;
  np->str = s;
  np->n = n;
  np->comps = (TreeNodePtr*) malloc(n * sizeof(TreeNodePtr));
  for (int i = 0; i < n; i++) {
    np->comps[i] = stack[top-n+i];
  }
  top -= (n-1);
  stack[top-1] = np;
}

void genEmpty() {
  stack[top++] = NULL;
}

void genNode(Categ cat, int n) {
  genNode3(cat, n, NULL);
}

void genIdent(char *tok_val) {
  genNode3(C_IDENT, 0, tok_val);
}

void genInt(char *tok_val) {
  genNode3(C_INT, 0, tok_val);
}

void genOpSymbol(Operator op) {
  genNode3(C_OPERATOR, 0, C_OPT_SYMBOL[op]);
}

void insertTopList() {
  TreeNodePtr t = stack[--top];
  TreeNodePtr s = stack[top-1];
  t->next = s;
  stack[top-1] = t;
}

void dumpNode(TreeNodePtr p) {
  printf("\ncateg: %i\n", p->categ);
  printf("n: %i\n", p->n);
  if(p->str)
    printf("str: %s\n", p->str);
}

void dumpTree(TreeNodePtr p) {
  if(p) {
    dumpNode(p);

    for(int i = 0; i < p->n; i++) {
      printf("\n%i -> ", i);
      dumpTree(p->comps[i]);
    }

    if(p->next)
      dumpTree(p->next);
  }else{
    printf("\nvazio\n");
  }
}
