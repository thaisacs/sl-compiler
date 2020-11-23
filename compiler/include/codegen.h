#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdbool.h>
#include <stdio.h>
#include "symbol_table.h"
#include "tree.h"
#include "util.h"
#include "type.h"

void processProgram(void*);
void processFuncDecl(TreeNodePtr, bool);
void processVarDecl(TreeNodePtr);
void processIteration(TreeNodePtr);
void processStatements(TreeNodePtr);

void genCode0(char*);
void genCode1(char*, int);
void genCode2(char*, char*);
void genCode3(char*, int, int);
void genCodeLabel(int);
void genCodeJump(char*, int);

void SemanticError();
int newLabel();

TypeDescrPtr processInt(TreeNodePtr);
TypeDescrPtr processVar(TreeNodePtr);
TypeDescrPtr processExpr(TreeNodePtr);
TypeDescrPtr processUnExpr(TreeNodePtr);
TypeDescrPtr processBinExpr(TreeNodePtr);
TypeDescrPtr predefBool();

char* getOp(TreeNodePtr);

bool relational(char *op);

#endif
