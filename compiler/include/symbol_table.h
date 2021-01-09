#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "type.h"

typedef enum {
  S_CONST = 1,
  S_VARIABLE,
  S_PARAMETER,
  S_FUNC_PARAM,
  S_FUNCTION,
  S_LABEL,
  S_TYPE
} SymbCateg;

typedef struct _descr {
  int displ;
  Passage pass;
  TypeDescrPtr type;
  TypeDescrPtr result;
  TypeDescrPtr params;
  int entLabel;
} Descr, *DescrPtr;

typedef struct _symbEntry {
  SymbCateg categ;
  char *ident;
  bool open;
  int level;
  struct _symbEntry *next;
  DescrPtr descr;
} SymbEntry, *SymbEntryPtr;

void insertSymbolTable(SymbEntryPtr);
void printSymbolTable();
void restoreSymbTable();

SymbEntryPtr newSymbEntry(SymbCateg, char*);
SymbEntryPtr searchSte(char*);
SymbEntryPtr searchLastFunction();

#endif
