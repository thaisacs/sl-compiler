#ifndef TREE_H
#define TREE_H

#define MAX_STACK_SIZE 100

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum {
  C_FUNCTION = 1,
  C_BLOCK,
  C_IDENT,
  C_BODY,
  C_VARS,
  C_ASSIGN,
  C_INT,
  C_FUNCTION_CALL,
  C_GOTO,
  C_VAR,
  C_WHILE,
  C_IF,
  C_OPERATOR,
  C_BIN_EXPR,
  C_UN_EXPR,
  C_REL_EXPR,
  C_EXPR,
  C_RETURN,
  C_LABEL,
  C_PARAM,
  C_TYPES
} Categ;

typedef enum {
  C_SUM,
  C_MUL,
  C_DIV,
  C_AND,
  C_MINUS,
  C_OR,
  C_NOT,
  C_LESS_EQUAL,
  C_LESS,
  C_EQUAL,
  C_DIFFERENT,
  C_GREATER_EQUAL,
  C_GREATER
} Operator;

typedef struct _treeNode {
  Categ categ;
  struct _treeNode *next;
  struct _treeNode **comps;
  char *str;
  int n;
} TreeNode, *TreeNodePtr;

//extern TreeNodePtr stack[MAX_STACK_SIZE];
//extern int top;

void *getTree();
void counts(void*, int*, int*, int*, int*, int*);
void genNode3(Categ, int, char*);
void genEmpty();
void genIdent(char*);
void genNode(Categ, int);
void genOpSymbol(Operator);
void genInt(char*);

void insertTopList();

char* copy_str(char*);

void dumpNode(TreeNodePtr);
void dumpTree(TreeNodePtr);

#endif
