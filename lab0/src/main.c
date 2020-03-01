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
		*out += num->bodyInt[i] * k;
	if (num->negative)
		*out *= -1.;
	return OK;
}

error read(unsigned int *bI, unsigned int *bO, char *str)	{
	if (bI == NULL || bO == NULL || str == NULL)
		return NULL_POINTER;

	FILE *fin = fopen(INPUT_FILE_NAME, "r");
	if (fin == NULL)
		return RUNTIME_ERROR;

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

/// Positive only
double integer_part(double const num) {
	if (num < 0) {
		printf("integer_part(): Range error.\n");
		exit(INVALID_ARGUMENT);
	}
	return (double)(unsigned int)num;
}

/// Positive only
double fraction_part(double const num) {
	if (num < 0) {
		printf("fraction_part(): Range error.\n");
		exit(INVALID_ARGUMENT);
	}
	return num - integer_part(num);
}

error init_num_with_dec(double const dec, unsigned int const base, Num *out) {
	if (out == NULL)
		return NULL_POINTER;
	if (base < MIN_BASE || base > MAX_BASE)
		return INVALID_ARGUMENT;

	out->base = base;

	if (dec == 0) {
		out->lenInt = 1;
		out->bodyInt = (unsigned int*)malloc(sizeof(unsigned int));
		out->bodyInt[0] = 0;
		out->lenFrac = 0;
		out->bodyFrac = NULL;
		return OK;
	}

	out->negative = (dec < 0) ? TRUE : FALSE;
	double const decPositive = (out->negative) ? dec * -1. : dec;

	// Convert integer part
	out->bodyInt = (unsigned int*)malloc(sizeof(unsigned int) * INPUT_MAX_LEN);
	out->lenInt = 0;
	for (unsigned int i = decPositive; i > 0; i /= base)
		out->bodyInt[out->lenInt++] = i % base;

	// Convert fraction part (warning: result is reverted)
	out->bodyFrac = (unsigned int*)malloc(sizeof(unsigned int) * INPUT_MAX_LEN);
	out->lenFrac = 0;
	double const decPositiveFrac = fraction_part(decPositive);
	for (double i = decPositiveFrac; i > 0; i *= base, i -= integer_part(i))
		out->bodyFrac[out->lenFrac++] = (unsigned int)(i * base);

	// Revert fraction part
	for (unsigned int i = 0; i < out->lenFrac / 2; ++i) {
		unsigned int const swapTargetToI = out->lenFrac - i - 2;
		unsigned int const temp = out->bodyFrac[i];
		out->bodyFrac[i] = out->bodyFrac[swapTargetToI];
		out->bodyFrac[swapTargetToI] = temp;
	}

	return OK;
}

error print(Num const *num) {
	if (num == NULL)
		return NULL_POINTER;
	if (num->bodyInt == NULL) // Check for uninitialised Num
		return INVALID_ARGUMENT;

	FILE *fout = fopen(OUTPUT_FILE_NAME, "w");
	if (fout == NULL)
		return RUNTIME_ERROR;

	for (unsigned int i = 0; i < num->lenInt; ++i) {
		unsigned int const revertedI = num->lenInt - i - 1;
		fprintf(fout, "%d", num->bodyInt[revertedI]);
	}

	if (num->lenFrac != 0)
		fprintf(fout, ".");
	for (unsigned int i = 0; i < num->lenFrac; ++i) {
		unsigned int const revertedI = num->lenFrac - i - 1;
		fprintf(fout, "%d", num->bodyFrac[revertedI]);
	}

	fclose(fout);
	return OK;
}

int main() {
	unsigned int base1, base2;
	char str[INPUT_MAX_LEN];

	error const readStatus = read(&base1, &base2, str);
	if (readStatus != OK)
		return readStatus;
	printf("Reading passed.\n");

	int isNegative = str[0] == '-' ? TRUE : FALSE;
	char *beginOfDigits = isNegative ? str + 1 : str;

	error const revertStatus = revert_str(beginOfDigits);
	if (revertStatus != OK)
		return revertStatus;
	printf("Reverting passed.\n");

	Num num = make_num();

	error const initStatus = init_num_with_str(str, base1, &num);
	if (initStatus != OK)
		return initStatus;
	printf("Initialisation passed.\n");

	double decNumber;

	error const decShowStatus = num_to_dec(&num, &decNumber);
	if (decShowStatus != OK)
		return decShowStatus;
	printf("Number before conversion = %f\n", decNumber);

	Num numConversed = make_num();

	error const convStatus = init_num_with_dec(decNumber, base2, &numConversed);
	if (convStatus != OK)
		return convStatus;
	printf("Conversion passed\n");

	double decNumberAfterConv;

	error const decShowStatus2 = num_to_dec(&numConversed, &decNumberAfterConv);
	if (decShowStatus2 != OK)
		return decShowStatus2;
	printf("Number after conversion = %f\n", decNumberAfterConv);

	if (decNumber != decNumberAfterConv) {
		printf("Numbers are not equal. Sth gone wrong.\n");
		return RUNTIME_ERROR;
	}
	printf("Numbers are equal. All right.\n");

	print(&numConversed);
	printf("Output printed to file.\n");

	return OK;
}
