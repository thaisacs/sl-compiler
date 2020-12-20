#include "type.h"

TypeDescrPtr newTypeDescr(TypeConstr c, TypePrimitive p, int s) {
  TypeDescrPtr type = (TypeDescrPtr) malloc(sizeof(TypeDescr));
  type->constr = c;
  type->prtv = p;
  type->size = s;
  type->next = NULL;
  return type;
}

TypeDescrPtr cloneTypeDescr(TypeDescrPtr t) {
  TypeDescrPtr tclone = (TypeDescrPtr) malloc(sizeof(TypeDescr));
  tclone->constr = t->constr;
  tclone->prtv = t->prtv;
  tclone->size = t->size;
  tclone->matriz = t->matriz;
  tclone->passage = t->passage;
  tclone->next = NULL;
  return tclone;
}

bool compatibleTypes(TypeDescrPtr a, TypeDescrPtr b) {
  if(a->size == b->size && a->prtv == b->prtv) {
    return true;
  }
  return false;
}

bool compatibleTypesBinOp(TypeDescrPtr op, TypeDescrPtr a, TypeDescrPtr b) {
  if(a->prtv == b->prtv && a->size == b->size) {
    return true;
  }
  return false;
}

bool compatibleTypesFunctionCall(TypeDescrPtr formals, TypeDescrPtr callparams) {
  for ( ; (formals!=NULL); formals=formals->next ) {
    if(!callparams)
      return false;
    if(formals->prtv != callparams->prtv)
      return false;
    callparams = callparams->next;
  }
  if(callparams)
    return false;
  return true;
}
