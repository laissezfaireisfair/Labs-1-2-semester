#include <stdlib.h>
#include <stdio.h>
#include "Errors.h"
#include "String.h"

error parse_expression(String const expr, List * nums, List * operations);

error count_parsed(List * nums, List * operations);

error count_expression(String const expr, long int & answer);

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
  error const readStatus = ask_string(fin, expr, maxInputSize);
  fclose(fin);
  if (readStatus != OK) {
    print_error(outputFilename, readStatus);
    return 2;
  }

  long int exprValue;
  error const statusCounting = count_expression(expr, &exprValue);
  deinit_str(&expr);
  if (statusCounting != OK) {
    print_error(outputFilename, statusCounting);
    return 3;
  }

  FILE *fout = fopen(outputFilename, "w");
  fprintf(fout, "%ld\n", exprValue);
  fclose(fout);
  return 0;
}
