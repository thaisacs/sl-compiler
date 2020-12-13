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
void processFunctionCall(TreeNodePtr);
void processReturn(TreeNodePtr);

TypeDescrPtr processFormals(TreeNodePtr, int*);

void genCode0(char*);
void genCode1(char*, int);
void genCode2(char*, char*);
void genCode3(char*, int, int);
void genCodeLabel(int, char*);
void genCodeEnfn(int, char*, int);
void genCodeJump(char*, int);

void SemanticError();
int newLabel();

TypeDescrPtr processInt(TreeNodePtr);
TypeDescrPtr processIdent(TreeNodePtr);
TypeDescrPtr processVar(TreeNodePtr);
TypeDescrPtr processExpr(TreeNodePtr);
TypeDescrPtr processUnExpr(TreeNodePtr);
TypeDescrPtr processBinExpr(TreeNodePtr);
TypeDescrPtr processRelational(TreeNodePtr);

TypeDescrPtr predefBool();
TypeDescrPtr predefInt();

TypeDescrPtr getOp(TreeNodePtr);

#endif
