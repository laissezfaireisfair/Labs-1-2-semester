#pragma once
#include <stdlib.h>
#include "List.h"

typedef unsigned int valT;

typedef struct _Stack {
  List body;
} Stack;

Stack make_stack();

void push(Stack *stack, valT const value);

valT pop(Stack *stack);

void delete_stack(Stack *stack);

int is_stack_empty(Stack const stack);
