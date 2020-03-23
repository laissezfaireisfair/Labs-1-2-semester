#include <stdlib.h>
#include <stdio.h>
#include "Errors.h"

void print_error(char const * const filename, error const err) {
	FILE *fout = fopen(filename, "w");
	if (fout == NULL)
		exit(RUNTIME_ERROR);
  switch(err) {
    case(OK):               fprintf(fout, "ok\n"); break;
    case(DIVISION_BY_ZERO): fprintf(fout, "division by zero\n"); break;
    case(NULL_POINTER):     fprintf(fout, "null pointer\n"); break;
    case(INVALID_ARGUMENT): fprintf(fout, "invalid argument\n"); break;
    case(BAD_INPUT):        fprintf(fout, "bad input\n"); break;
    case(RUNTIME_ERROR):    fprintf(fout, "runtime error\n"); break;
    case(LENGTH_ERROR):     fprintf(fout, "length error\n"); break;
  }
	fclose(fout);
}
