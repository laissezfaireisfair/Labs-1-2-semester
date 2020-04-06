#pragma once
#include <stdlib.h>

typedef struct _ListElem {
  unsigned int value;
  struct _ListElem *next;
} ListElem;

typedef struct _List {
  ListElem *head;
} List;

List make_list();

void push_front(List *list, unsigned int const value);

unsigned int pop_back(List *list);

void delete_list(List *list);
