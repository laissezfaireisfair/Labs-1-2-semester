#include "Queue.h"

Queue make_queue() {
  Queue queue;
  queue.body = make_list();
  return queue;
}

void push_to_queue(Queue *queue, valType const value) {
  push_front(&queue->body, value);
}

valType pop_from_queue(Queue *queue) {
  return pop_back(&queue->body);
}

void delete_queue(Queue *queue) {
  delete_list(&queue->body);
}

int is_queue_empty(Queue const queue) {
  if (queue.body.head == NULL)
    return 1;
  return 0;
}
