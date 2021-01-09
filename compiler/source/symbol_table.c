#include "symbol_table.h"

SymbEntryPtr SymbolTable = NULL;

SymbEntryPtr newSymbEntry(SymbCateg categ, char *id) {
  SymbEntryPtr symb_entry = (SymbEntryPtr) malloc(sizeof(SymbEntry));
  symb_entry->categ = categ;
  symb_entry->ident = id;
  symb_entry->descr = (DescrPtr) malloc(sizeof(Descr));
  symb_entry->next = NULL;
  return symb_entry;
}

void insertSymbolTable(SymbEntryPtr entry) {
  entry->open = true;
  if(SymbolTable != NULL)
    entry->next = SymbolTable;
  SymbolTable = entry;
}

void printSymbolTable() {
  printf("======================\n");
  SymbEntryPtr p = SymbolTable;
  for ( ; (p!=NULL); p=p->next ) {
    switch(p->categ) {
      case S_CONST:
        printf("const ");
      case S_VARIABLE:
        printf("var ");
    }
    printf("%s %i %i %i\n", p->ident, p->level, p->descr->displ, p->categ);
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

SymbEntryPtr searchLastFunction() {
  SymbEntryPtr p = SymbolTable;
  for ( ; (p!=NULL); p=p->next ) {
    if(p->categ == S_FUNCTION && p->open) {
      return p;
    }
  }
  return NULL;
}

void restoreSymbTable() {
  int cleanLevel = SymbolTable->level;
  SymbEntryPtr p = SymbolTable;
  SymbEntryPtr q = NULL;

  while(p != NULL && p->level == cleanLevel) {
    if(p->categ != S_FUNCTION) {
      SymbEntryPtr e = p;
      if(q)
        q->next = p->next;
      else
        SymbolTable = p->next;
      p = p->next;
      free(e);
    }else {
      p->open = false;
      q = p;
      p = p->next;
    }
  }
}
