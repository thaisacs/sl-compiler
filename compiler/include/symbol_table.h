#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdlib.h>
#include <codegen.h>

typedef enum {
  S_CONST = 1,
  S_VARIABLE,
  S_PARAMETER,
  S_FUNCTION,
  S_LABEL,
  S_TYPE
} SymbCateg;

typedef struct _symbEntry {
  SymbCateg categ;
  char *ident;
  int level;
  struct _symbEntry *next;
} SymbEntry, *SymbEntryPtr;

SymbEntryPtr newSymbEntry(SymbCateg, char*);
void insertSymbolTable(SymbEntryPtr);
void printSymbolTable();
SymbEntryPtr searchSte(char*);

#endif
