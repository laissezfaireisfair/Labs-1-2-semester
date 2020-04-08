#include <stdlib.h>
#include <stdio.h>
#include "Errors.h"
#include "String.h"
#include "Stack.h"

// Comparsion of "bool" variables
int is_bool_equal(int const num1, int const num2) {
  if ((num1 == 0 && num2 == 0) || (num1 != 0 && num2 != 0))
    return 1;
  return 0;
}

error apply_operation(int const num1, int const num2, char const sign, int *answ) {
  if (answ == NULL)
    return NULL_POINTER;

  switch(sign) {
    case('+'): *answ = num1 + num2; return OK;
    case('-'): *answ = num1 - num2; return OK;
    case('*'): *answ = num1 * num2; return OK;
    case('/'): {
      if (num2 == 0)
        return DIVISION_BY_ZERO;
      *answ = num1 / num2;
      return OK;
    }
    default: return INVALID_ARGUMENT;
  }
}

int type_of_symbol(char const symbol) {
  if (symbol >= '0' && symbol <= '9')
    return 1;
  if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/')
    return 2;
  if (symbol == '(')
    return 3;
  if (symbol == ')')
    return 4;
  return 0;
}

error parse_expression(String const expr, Stack * nums, Stack * operations) {
  if (nums == NULL || operations == NULL)
    return NULL_POINTER;
  if (expr.body == NULL)
    return INVALID_ARGUMENT;

  for (unsigned int i = 0; i < expr.length; ++i) {
    char const symbol = expr.body[i];
    int const type = type_of_symbol(symbol);
    // TODO: Handle symbol
  }

  return OK;
}

error count_parsed(Stack nums, Stack operations, int *answ) {
  if (answ == NULL)
    return NULL_POINTER;
  if (is_stack_empty(nums) || is_stack_empty(operations))
    return INVALID_ARGUMENT;

  for (*answ = pop(&nums); !is_stack_empty(nums) && !is_stack_empty(operations);) {
    int const num2 = pop(&nums);
    char const sign = (char)(pop(&operations));
    error const statusCounting = apply_operation(*answ, num2, sign, answ);
    if (statusCounting != OK)
      return statusCounting;
  }

  // Check if one of stacks still have elements:
  if (!is_bool_equal(is_stack_empty(nums), is_stack_empty(operations)))
    return INVALID_ARGUMENT;

  return OK;
}

error count_expression(String const expr, int *answ) {
  if (answ == NULL)
    return NULL_POINTER;

  Stack nums = make_stack(), operations = make_stack();

  error const parseStatus = parse_expression(expr, &nums, &operations);
  if (parseStatus != OK)
    return parseStatus;

  error const countStatus = count_parsed(nums, operations, answ);
  delete_stack(&nums);
  delete_stack(&operations);
  return countStatus;
}

int main() {
  char const * const inputFilename  = "input.txt";
  char const * const outputFilename = "output.txt";
  unsigned int const maxInputSize   = 1000;

  FILE *fin = fopen(inputFilename, "r");
  if (fin == NULL) {
    print_error(inputFilename, RUNTIME_ERROR);
    return 1;
  }

  String expr = make_str();
  error const readStatus = ask_string(fin, &expr, maxInputSize);
  fclose(fin);
  if (readStatus != OK) {
    print_error(outputFilename, readStatus);
    return 2;
  }

  int exprValue;
  error const statusCounting = count_expression(expr, &exprValue);
  deinit_str(&expr);
  if (statusCounting != OK) {
    print_error(outputFilename, statusCounting);
    return 3;
  }

  FILE *fout = fopen(outputFilename, "w");
  fprintf(fout, "%d\n", exprValue);
  fclose(fout);
  return 0;
}
