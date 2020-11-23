#include "type.h"

TypeDescrPtr newTypeDescr(TypeConstr c, TypePrimitive p, int s) {
  TypeDescrPtr type = (TypeDescrPtr) malloc(sizeof(TypeDescr));
  type->constr = c;
  type->prtv = p;
  type->size = s;
  return type;
}

bool compatibleTypes(TypeDescrPtr a, TypeDescrPtr b) {
  //printf("%d %d %d\n", a->size, a->constr, a->prtv);
  //printf("%d %d %d\n", b->size, b->constr, b->prtv);
  if(a->size == b->size && a->constr == b->constr
      && a->prtv == b->prtv) {
    return true;
  }
  printf("não\n");
  return false;
}
