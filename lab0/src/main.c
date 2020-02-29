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
		if (i == INPUT_MAX_LEN - 1 || j == INPUT_MAX_LEN - 1) {
			free(num->bodyInt);
			free(num->bodyFrac);
			return LENGTH_ERROR;
		}
		if (str[i] < '0' || str[i] > '0' + base - 1) {
			free(num->bodyInt);
			free(num->bodyFrac);
			return INVALID_ARGUMENT;
		}
		num->bodyFrac[j] = str[i] - '0';
		num->lenFrac = j + 1;
	}

	if (str[i] == 0)
		return OK;
	if (i == INPUT_MAX_LEN - 2 && str[i] == '.') { // Point as last symbol
		free(num->bodyInt);
		free(num->bodyFrac);
		return INVALID_ARGUMENT;
	}

	// Get integer part of num:
	i += 1; // Skip symbol of point
	for (unsigned int j = 0; str[i] != 0; ++i, ++j) {
		if (i == INPUT_MAX_LEN - 1 || j == INPUT_MAX_LEN - 1) {
			free(num->bodyInt);
			free(num->bodyFrac);
			return LENGTH_ERROR;
		}
		if (str[i] < '0' || str[i] > '0' + base - 1) {
			free(num->bodyInt);
			free(num->bodyFrac);
			return INVALID_ARGUMENT;
		}
		num->bodyInt[j] = str[i] - '0';
		num->lenInt = j + 1;
	}

	if (num->lenInt == 0 && num->lenFrac == 0) {
		free(num->bodyInt);
		free(num->bodyFrac);
		return INVALID_ARGUMENT;
	}

	return OK;
}

error num_to_dec(Num const *num, double *out) {
	if (num == NULL || out == NULL)
		return NULL_POINTER;
	if (num->base == 0) // Test for uninitialised Num
		return INVALID_ARGUMENT;
	*out = 0.;
	double j = 1. / num->base;
	for (int i = num->lenFrac - 1; i >= 0; --i, j /= num->base)
		*out += num->bodyFrac[(unsigned int)i] * j;
	for (unsigned int i = 0, k = 1; i < num->lenInt; ++i, k *= num->base)
		*out += num->bodyFrac[i] * k;
	return OK;
}

error read(unsigned int *bI, unsigned int *bO, char *str)	{
	if (bI == NULL || bO == NULL || str == NULL)
		return NULL_POINTER;

	FILE *fin = fopen(INPUT_FILE_NAME, "r");

	int const baseReadStatus = fscanf(fin, "%u %u\n", bI, bO);

	if (baseReadStatus == EOF || baseReadStatus == 0)
		return BAD_INPUT;

	int pointPassed = FALSE;

	for (unsigned int i = 0; i < INPUT_MAX_LEN; ++i) {
		char symbol;
		int symbolReadStatus = fscanf(fin, "%c", &symbol);
		if (symbolReadStatus == 0)
			return BAD_INPUT;
		if (symbolReadStatus == EOF || symbol == '\n') {
			if (i == 0)
				return BAD_INPUT;
			else {
				str[i] = 0;
				break;
			}
		}
		if (symbol == '-' && i == 0) {
			str[i] = symbol;
			continue;
		}
		if (symbol == '.') {
			if (pointPassed == FALSE) {
				pointPassed = TRUE;
				str[i] = symbol;
				continue;
			}
			return BAD_INPUT;
		}
		if (symbol < '0' || symbol > '0' + *bI - 1)
				return BAD_INPUT;
		str[i] = symbol;
	}

	fclose(fin);
	return OK;
}

error revert_str(char *str) {
	if (str == NULL)
		return NULL_POINTER;
	unsigned int length = 1;
	for (;str[length - 1] != 0; ++length);
	for (unsigned int i = 0; i < length / 2; ++i) {
		char temp = str[i];
		str[i] = str[length - i - 2];
		str[length - i - 2] = temp;
	}
	return OK;
}

int main() {
	unsigned int base1, base2;
	char str[INPUT_MAX_LEN];

	error readStatus = read(&base1, &base2, str);
	if (readStatus != OK)
		return readStatus;
	printf("Reading passed.\n");

	int isNegative = str[0] == '-' ? TRUE : FALSE;
	char *beginOfDigits = isNegative ? str + 1 : str;

	error revertStatus = revert_str(beginOfDigits);
	if (revertStatus != OK)
		return revertStatus;
	printf("Reverting passed.\n");

	Num num = make_num();

	error initStatus = init_num_with_str(str, base1, &num);
	if (initStatus != OK)
		return initStatus;
	printf("Initialisation passed.\n");

	double decNumber;

	error conversionStatus = num_to_dec(&num, &decNumber);
	if (conversionStatus != OK)
		return conversionStatus;
	printf("Conversion passed. number = %f\n", decNumber);

	return OK;
}
