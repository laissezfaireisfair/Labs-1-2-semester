#pragma once
#include <stdlib.h>

enum errors {OK, DIVISION_BY_ZERO, NULL_POINTER, INVALID_ARGUMENT, BAD_INPUT,
	 						RUNTIME_ERROR, LENGTH_ERROR};
typedef enum errors error;

void print_error(char const * const filename, error const err);
