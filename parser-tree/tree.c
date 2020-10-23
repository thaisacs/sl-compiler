#include "tree.h"
#include <stdio.h>
#include <string.h>

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
  if(p->categ == C_FUNCTION)
    (*functions)++;
}

void counts(void *p, int *functions, int *funcalls,
    int *whiles, int *ifs, int *bin) {

  *functions = 0;
  *funcalls = 0;
  *whiles = 0;
  *ifs = 0;
  *bin = 0;

  //test(p, functions, funcalls, whiles, ifs, bin);
  dumpTree(p);
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

void insertTopList() {
  TreeNodePtr t = stack[--top];
  TreeNodePtr s = stack[top-1];
  t->next = s;
  stack[top-1] = t;
}

void dumpNode(TreeNodePtr p) {
  printf("categ: %i\n", p->categ);
  printf("n: %i\n", p->n);
  if(p->str)
    printf("str: %s\n", p->str);
}

void dumpTree(TreeNodePtr p) {
  //dumpNode(p);

  dumpNode(p->comps[0]);

  //TreeNodePtr k = p->next;
  //while(k) {
  //  dumpNode(k);
  //  k = k->next;
  //}

}
