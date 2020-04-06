#pragma once
#include <stdlib.h>

typedef char valType;

typedef struct _ListElem {
  valType value;
  struct _ListElem *next;
} ListElem;

typedef struct _List {
  ListElem *head;
} List;

List make_list();

void push_front(List *list, valType const value);

unsigned int pop_back(List *list);

void delete_list(List *list);
