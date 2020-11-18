#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdbool.h>
#include <stdio.h>
#include "symbol_table.h"
#include "tree.h"
#include "util.h"

typedef enum { P_VALUE = 1, P_VARIABLE } Passage;

typedef enum {
  T_PREDEF = 1,
  T_ARRAY,
  T_FUNCTION
} TypeConstr;


typedef struct _typeDescr {
  TypeConstr constr;
  int size;
} TypeDescr, *TypeDescrPtr;

void processProgram(void*);
void processFuncDecl(TreeNodePtr, bool);
void processVarDecl(TreeNodePtr);

void genCode0(char*);
void genCode1(char*, int);
void genCode2(char*, char*);
void genCode3(char*, int, int);

#endif
