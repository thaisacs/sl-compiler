#ifndef UTIL_H
#define UTIL_H

#include "tree.h"
#include <string.h>
#include "stdio.h"
#include "stdlib.h"

//typedef struct _labelEntry {
//  char *label;
//  int ident;
//  struct _labelEntry *next;
//} LabelEntry, *LabelEntryPtr;

TreeNodePtr invertList(TreeNodePtr);
//LabelEntryPtr newLabelEntry(int, char*);
//void insertLabelTable(LabelEntryPtr);
//int searchLte(char*);
//void printLabels();

#endif
