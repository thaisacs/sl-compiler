#ifndef TYPE_H
#define TYPE_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum { P_VALUE = 1, P_VARIABLE } Passage;

typedef enum {
  T_PREDEF = 1,
  T_ARRAY,
  T_FUNCTION
} TypeConstr;

typedef enum {
  T_INT = 1,
  T_BOOL,
  T_VOID,
} TypePrimitive;

typedef struct _typeDescr {
  TypeConstr constr;
  TypePrimitive prtv;
  Passage passage;
  int size;
  int matriz;
  struct _typeDescr *next;
} TypeDescr, *TypeDescrPtr;

bool compatibleTypes(TypeDescrPtr, TypeDescrPtr);
bool compatibleTypesBinOp(TypeDescrPtr, TypeDescrPtr, TypeDescrPtr);
bool compatibleTypesFunctionCall(TypeDescrPtr, TypeDescrPtr);

TypeDescrPtr cloneTypeDescr(TypeDescrPtr);
TypeDescrPtr newTypeDescr(TypeConstr, TypePrimitive, int);

#endif
