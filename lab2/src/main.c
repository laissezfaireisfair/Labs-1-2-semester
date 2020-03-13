#include <stdlib.h>
#include <stdio.h>

int const          TRUE             = 1;
int const          FALSE            = 0;
char const * const INPUT_FILE_NAME  = "in.txt";
char const * const OUTPUT_FILE_NAME = "out.txt";

enum errors {OK, DIVISION_BY_ZERO, NULL_POINTER, INVALID_ARGUMENT, BAD_INPUT,
	 						RUNTIME_ERROR, LENGTH_ERROR};
typedef enum errors error;

void print_error() {
	FILE *fout = fopen(OUTPUT_FILE_NAME, "w");
	if (fout == NULL)
		exit(RUNTIME_ERROR);
	fprintf(fout, "bad input\n");
	fclose(fout);
}

int main(void) {
    return OK;
}
