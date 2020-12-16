#include "type.h"

TypeDescrPtr newTypeDescr(TypeConstr c, TypePrimitive p, int s) {
  TypeDescrPtr type = (TypeDescrPtr) malloc(sizeof(TypeDescr));
  type->constr = c;
  type->prtv = p;
  type->size = s;
  type->next = NULL;
  return type;
}

bool compatibleTypes(TypeDescrPtr a, TypeDescrPtr b) {
  //if(a->size == b->size && a->constr == b->constr
  //    && a->prtv == b->prtv) {
    return true;
  //}
  //return false;
}

bool compatibleTypesBinOp(TypeDescrPtr op, TypeDescrPtr a, TypeDescrPtr b) {
  //if(a->size == b->size && a->constr == b->constr
  //    && a->prtv == b->prtv) {
    return true;
  //}
  //return false;
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
