#pragma once
#include <stdlib.h>
#include "List.h"

typedef struct _Queue {
  List body;
} Queue;

Queue make_queue();

void push_to_queue(Queue *queue, valType const value);

valType pop_from_queue(Queue *queue);

void delete_queue(Queue *queue);

int is_queue_empty(Queue const queue);
