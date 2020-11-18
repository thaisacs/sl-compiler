#include "symbol_table.h"

SymbEntryPtr SymbolTable = NULL;

SymbEntryPtr newSymbEntry(SymbCateg categ, char *id) {
  SymbEntryPtr symb_entry = (SymbEntryPtr) malloc(sizeof(SymbEntry));
  symb_entry->categ = categ;
  symb_entry->ident = id;
  return symb_entry;
}

void insertSymbolTable(SymbEntryPtr entry) {
  if(SymbolTable != NULL)
    entry->next = SymbolTable;
  SymbolTable = entry;
}

void printSymbolTable() {
  printf("======================\n");
  SymbEntryPtr p = SymbolTable;
  while(p) {
    switch(p->categ) {
      case S_CONST:
        printf("const ");
      case S_VARIABLE:
        printf("var ");
    }
    printf("%s %i\n", p->ident, p->level);
    p = p->next;
  }
  printf("======================\n");
}

SymbEntryPtr searchSte(char *id) {
  SymbEntryPtr p = SymbolTable;
  for ( ; (p!=NULL); p=p->next ) {
    if(!strcmp(p->ident, id)) {
      return p;
    }
  }
  return NULL;
}
