#ifndef TREE_H
#define TREE_H

#define MAX_COMPS 5
#define MAX_STACK_SIZE 10

#include <stdlib.h>

typedef enum {
  C_FUNCTION = 1,
  C_BLOCK,
  C_IDENT,
  C_BODY,
  C_VARS,
  C_ASSIGN,
  C_INT
} Categ;

typedef struct _treeNode {
  Categ categ;
  struct _treeNode *next;
  struct _treeNode **comps;
  char *str;
  int n;
} TreeNode, *TreeNodePtr;

extern TreeNodePtr stack[MAX_STACK_SIZE];
extern int top;

void *getTree();
void counts(void*, int*, int*, int*, int*, int*);
void genNode3(Categ, int, char*);
void genEmpty();
void genIdent(char*);
void genNode(Categ, int);
void genInt(char*);
int copy_int(char*);

void insertTopList();

void dumpNode(TreeNodePtr);
void dumpTree(TreeNodePtr);

char* copy_str(char*);

#endif
