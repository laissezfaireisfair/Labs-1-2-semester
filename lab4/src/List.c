#include <assert.h>
#include "List.h"

List make_list() {
  List list;
  list.head = NULL;
  return list;
}

void push_front(List *list, valType const value) {
  ListElem *newElem = (ListElem*)malloc(sizeof(ListElem));
  newElem->value = value;
  newElem->next = list->head;
  list->head = newElem;
}

valType pop_back(List *list) {
  assert(list->head != NULL);

  // Deleting last element case
  if (list->head->next == NULL) {
    valType const lastValue = list->head->value;
    free(list->head);
    list->head = NULL;
    return lastValue;
  }

  // Deleting non-last element case
  ListElem *preLast;
  for (preLast = list->head; preLast->next->next != NULL; preLast = preLast->next);
  valType const lastValue = preLast->next->value;
  free(preLast->next);
  preLast->next = NULL;
  return lastValue;
}

valType pop_front(List *list) {
  assert(list->head != NULL);

  valType const lastValue = list->head->value;
  ListElem *newHead = list->head->next;
  free(list->head);
  list->head = newHead;
  return lastValue;
}

void delete_list(List *list) {
  for (ListElem *i = list->head; i != NULL;) {
    ListElem *next = i->next;
    free(i);
    i = next;
  }
}
