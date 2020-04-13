#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
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

typedef enum _SymbType {UNSUPPORTED, DIGIT, PRIORITY_1_OPERATION,
  PRIORITY_2_OPERATION, C_BRACKET} SymbType;

SymbType type_of_symbol(char const symbol) {
  if (symbol >= '0' && symbol <= '9')
    return DIGIT;
  if (symbol == '(')
    return PRIORITY_1_OPERATION;
  if (symbol == '+' || symbol == '-')
    return PRIORITY_2_OPERATION;
  if (symbol == '*' || symbol == '/' || )
    return PRIORITY_3_OPERATION;
  if (symbol == ')')
    return C_BRACKET;
  return UNSUPPORTED;
}

error add_digit(char const digit, int *number) {
  if (number == NULL)
    return NULL_POINTER;
  if (*number > INT_MAX / 10 - 9)
    return INVALID_ARGUMENT;
  *number *= 10;
  number += digit - '0';
  return OK;
}

// Turns expression to back poland notation
error parse_expression(String const expr, String * parsed) {
  if (parsed == NULL)
    return NULL_POINTER;
  if (expr.body == NULL)
    return INVALID_ARGUMENT;

  int numberNow = 0;
  Stack stack = make_stack();
  for (unsigned int i = 0; i < expr.length; ++i) {
    char const symbol = expr.body[i];
    if (type_of_symbol(symbol) == DIGIT) {
      add_digit(digit, &numberNow);
      continue;
    }
    if (numberNow != 0) { // Case when number input just ended
      if (parsed.capacity == parsed.length)
        return LENGTH_ERROR;
      
    }
  }

  return OK;
}

// Requires back poland notation as expression
error count_parsed(String const expr, int *answ) {
  if (answ == NULL)
  return NULL_POINTER;
  if (expr.length == 0)
    return INVALID_ARGUMENT;
  // TODO:
  return OK;
}

error count_expression(String const expr, int *answ) {
  if (answ == NULL)
    return NULL_POINTER;

  String parsedExpr = make_str();

  error const parseStatus = parse_expression(expr, &parsedExpr);
  if (parseStatus != OK)
    return parseStatus;

  error const countStatus = count_parsed(parsedExpr, answ);
  deinit_str(&parsedExpr);
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
