#include "util.h"

TreeNodePtr invertList(TreeNodePtr p) {
  TreeNodePtr pvars = NULL, r;
  while (p != NULL) {
    r = p->next;
    p->next = pvars;
    pvars = p;
    p = r;
  }
  return pvars;
}
