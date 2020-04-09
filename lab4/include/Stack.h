#pragma once
#include <stdlib.h>
#include "List.h"

typedef struct _Stack {
  List body;
} Stack;

Stack make_stack();

void push_to_stack(Stack *stack, valType const value);

valType pop_from_stack(Stack *stack);

void delete_stack(Stack *stack);

int is_stack_empty(Stack const stack);
