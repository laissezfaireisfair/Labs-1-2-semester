#include <stdio.h>
#include <stdlib.h>

int const          TRUE             = 1;
int const          FALSE            = 0;
unsigned int const MAX_BASE         = 16;
unsigned int const MIN_BASE         = 2;
unsigned int const INPUT_MAX_LEN    = 13;
char const * const INPUT_FILE_NAME  = "in.txt";
char const * const OUTPUT_FILE_NAME = "out.txt";

enum errors {OK, DIVISION_BY_ZERO, NULL_POINTER, INVALID_ARGUMENT, BAD_INPUT,
	 						RUNTIME_ERROR, LENGTH_ERROR};
typedef enum errors error;

typedef struct _Num {
	unsigned char negative;
	unsigned int  base;
  unsigned int* bodyInt;
  unsigned int  lenInt;
  unsigned int* bodyFrac;
  unsigned int  lenFrac;
} Num;

Num make_num() {
	Num num;
	num.base     = 0;
	num.negative = FALSE;
	num.bodyInt  = NULL;
	num.lenInt   = 0;
	num.bodyFrac = NULL;
	num.lenFrac  = 0;
	return num;
}

error delete_num(Num *num) {
	if (num == NULL)
		return NULL_POINTER;
	free(num->bodyInt);
	free(num->bodyFrac);
	return OK;
}

/// String should be like "0.1" or "-0.1" where 0 is less significant digit
error init_num_with_str(char const *str, unsigned int const base, Num *num) {
	if (str == NULL || num == NULL)
		return NULL_POINTER;
	if (base > MAX_BASE || base < MIN_BASE)
		return INVALID_ARGUMENT;

	num->base     = base;
	num->bodyInt  = (unsigned int*)malloc(sizeof(unsigned int) * INPUT_MAX_LEN);
	num->bodyFrac = (unsigned int*)malloc(sizeof(unsigned int) * INPUT_MAX_LEN);

	unsigned int i = 0; // Iter in str[]
	if (str[0] == '-') {
		num->negative = TRUE;
		i += 1; // Skip minus symbol
	} else
		num->negative = FALSE;

	// Get fraction part of num:
	num->lenFrac = 0;
	for (unsigned int j = 0; str[i] != 0 && str[i] != '.'; ++i, ++j) {
		if (i == INPUT_MAX_LEN - 1 || j == INPUT_MAX_LEN - 1)
			return LENGTH_ERROR;
		if (str[i] < '0' || str[i] > '0' + base - 1)
			return INVALID_ARGUMENT;
		num->bodyFrac[j] = str[i] - '0';
		num->lenFrac = j + 1;
	}

	if (num->lenFrac == 0)
		return INVALID_ARGUMENT;
	if (str[i] == 0)
		return OK;
	if (i == INPUT_MAX_LEN - 2 && str[i] == '.') // Point as last symbol
		return INVALID_ARGUMENT;

	// Get integer part of num:
	i += 1; // Skip symbol of point
	for (unsigned int j = 0; str[i] != 0; ++i, ++j) {
		if (i == INPUT_MAX_LEN - 1 || j == INPUT_MAX_LEN - 1)
			return LENGTH_ERROR;
		if (str[i] < '0' || str[i] > '0' + base - 1)
			return INVALID_ARGUMENT;
		num->bodyInt[j] = str[i] - '0';
		num->lenInt = j + 1;
	}

	if (num->lenInt == 0)
		return INVALID_ARGUMENT;

	return OK;
}

error num_to_dec(Num const *num, double *out) {
	if (num == NULL || out == NULL)
		return NULL_POINTER;
	if (num->base == 0) // Test for uninitialised Num
		return INVALID_ARGUMENT;
	*out = 0.;
	double j = 1. / num->base;
	for (unsigned int i = 0; i < num->lenFrac; ++i, j /= num->base)
		*out += num->bodyFrac[i] * j;
	for (unsigned int i = 0, k = num->base; i < num->lenInt; ++i, k *= num->base)
		*out += num->bodyFrac[i] * k;
	return OK;
}

int main() {
	return OK;
}
