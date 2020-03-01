#include <stdio.h>
#include <stdlib.h>

int const          TRUE             = 1;
int const          FALSE            = 0;
unsigned int const MAX_BASE         = 16;
unsigned int const MIN_BASE         = 2;
unsigned int const INPUT_MAX_LEN    = 13;
unsigned int const FRAC_OUT_MAX_LEN = 12;
char const * const INPUT_FILE_NAME  = "in.txt";
char const * const OUTPUT_FILE_NAME = "out.txt";

enum errors {OK, DIVISION_BY_ZERO, NULL_POINTER, INVALID_ARGUMENT, BAD_INPUT,
	 						RUNTIME_ERROR, LENGTH_ERROR};
typedef enum errors error;

typedef struct _Num {
	unsigned char negative;
	unsigned int  base;
	unsigned int  lenInt;
	unsigned int  lenFrac;
  char          *bodyInt;  // It is NOT a string (just byte array)
  char          *bodyFrac; // It is NOT a string (just byte array)
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

int is_digit(char const symbol, unsigned int const base) {
	if (symbol >= '0' && symbol <= '9') {
		if ((unsigned int)(symbol - '0') < base)
			return TRUE;
		else
			return FALSE;
	}

	if (symbol >= 'A' && symbol <= 'Z') {
		if ((unsigned int)(symbol - 'A' + 10) < base)
			return TRUE;
		else
			return FALSE;
	}

	return FALSE;
}

error to_digit(char const symbol, unsigned int const base, char *digit) {
	if (digit == NULL)
		return NULL_POINTER;

	if (symbol >= '0' && symbol <= '9') {
		if ((unsigned int)(symbol - '0') < base) {
			*digit = symbol - '0';
			return OK;
		} else
			return INVALID_ARGUMENT;
	}

	if (symbol >= 'A' && symbol <= 'Z') {
		if ((unsigned int)(symbol - 'A' + 10) < base) {
			*digit = symbol - 'A' + 10;
			return OK;
		} else
			return INVALID_ARGUMENT;
	}

	return INVALID_ARGUMENT;
}

/// String should be like "0.1" or "-0.1" where 0 is less significant digit
error init_num_with_str(char const *str, unsigned int const base, Num *num) {
	if (str == NULL || num == NULL)
		return NULL_POINTER;
	if (base > MAX_BASE || base < MIN_BASE)
		return INVALID_ARGUMENT;

	num->base     = base;
	num->bodyInt  = (char*)malloc(sizeof(char) * INPUT_MAX_LEN);
	if (num->bodyInt == NULL)
		return RUNTIME_ERROR;
	num->bodyFrac = (char*)malloc(sizeof(char) * INPUT_MAX_LEN);
	if (num->bodyFrac == NULL) {
		free(num->bodyInt);
		return RUNTIME_ERROR;
	}

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
		if (to_digit(str[i], base, num->bodyFrac + j) != OK) {
			free(num->bodyInt);
			free(num->bodyFrac);
			return INVALID_ARGUMENT;
		}

		num->lenFrac = j + 1;
	}

	if (str[i] == 0) { // If there is no point, then it was integer part: swap it
		char *tempPtr = num->bodyFrac;
		num->bodyFrac         = num->bodyInt;
		num->bodyInt          = tempPtr;
		unsigned int temp     = num->lenFrac;
		num->lenFrac          = num->lenInt;
		num->lenInt           = temp;
		return OK;
	}
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
		if (to_digit(str[i], base, num->bodyInt + j) != OK) {
			free(num->bodyInt);
			free(num->bodyFrac);
			return INVALID_ARGUMENT;
		}

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
		if (!is_digit(symbol, *bI))
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
		out->bodyInt = (char*)malloc(sizeof(char));
		if (out->bodyInt == NULL)
			return RUNTIME_ERROR;
		out->bodyInt[0] = 0;
		out->lenFrac = 0;
		out->bodyFrac = NULL;
		return OK;
	}

	out->negative = (dec < 0) ? TRUE : FALSE;
	double const decPositive = (out->negative) ? dec * -1. : dec;

	// Convert integer part
	out->bodyInt = (char*)malloc(sizeof(char) * INPUT_MAX_LEN);
	if (out->bodyInt == NULL)
		return RUNTIME_ERROR;
	out->lenInt = 0;
	for (unsigned int i = decPositive; i > 0; i /= base)
		out->bodyInt[out->lenInt++] = i % base;

	// Convert fraction part (warning: result is reverted)
	out->bodyFrac = (char*)malloc(sizeof(char) * INPUT_MAX_LEN);
	if (out->bodyFrac == NULL) {
		free(out->bodyInt);
		return RUNTIME_ERROR;
	}
	out->lenFrac = 0;
	double const decPositiveFrac = fraction_part(decPositive);
	for (double i = decPositiveFrac; i > 0; i *= base, i -= integer_part(i)) {
		out->bodyFrac[out->lenFrac++] = (unsigned int)(i * base);
		if (out->lenFrac == FRAC_OUT_MAX_LEN)
			break;
	}

	// Revert fraction part
	for (unsigned int i = 0; i < out->lenFrac / 2; ++i) {
		unsigned int const swapTargetToI = out->lenFrac - i - 2;
		unsigned int const temp = out->bodyFrac[i];
		out->bodyFrac[i] = out->bodyFrac[swapTargetToI];
		out->bodyFrac[swapTargetToI] = temp;
	}

	return OK;
}

/// Will use trash symbols if MAX_BASE > 26
error print_digit(char const digit, FILE* fout) {
	if (digit < 0 || (unsigned int)(digit) >= MAX_BASE)
		return INVALID_ARGUMENT;
	char symbol;
	if (digit < 10)
		symbol = digit + '0';
	else
		symbol = digit - 10 + 'A';
	fprintf(fout, "%c", symbol);
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
		error const status = print_digit(num->bodyInt[revertedI], fout);
		if (status != OK)
			return status;
	}

	if (num->lenFrac != 0)
		fprintf(fout, ".");
	for (unsigned int i = 0; i < num->lenFrac; ++i) {
		unsigned int const revertedI = num->lenFrac - i - 1;
		error const status = print_digit(num->bodyFrac[revertedI], fout);
		if (status != OK)
			return status;
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

	// LSB should be at the beginning
	char *beginOfDigits = str[0] == '-' ? str + 1 : str;
	error const revertStatus = revert_str(beginOfDigits);
	if (revertStatus != OK)
		return revertStatus;

	Num num = make_num();
	error const initStatus = init_num_with_str(str, base1, &num);
	if (initStatus != OK)
		return initStatus;

	double decNumber;
	error const decShowStatus = num_to_dec(&num, &decNumber);
	if (decShowStatus != OK)
		return decShowStatus;

	Num numConversed = make_num();
	error const convStatus = init_num_with_dec(decNumber, base2, &numConversed);
	if (convStatus != OK)
		return convStatus;


	error const printStatus = print(&numConversed);;
	if (printStatus != OK)
		return printStatus;

	return OK;
}
