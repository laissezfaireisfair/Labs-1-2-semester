#include <stdio.h>
#include <stdlib.h>

int const          TRUE          = 1;
int const          FALSE         = 0;
unsigned int const MAX_BASE      = 16;
unsigned int const MIN_BASE      = 2;
unsigned int const INPUT_MAX_LEN = 13;

enum errors {OK, DIVISION_BY_ZERO, NULL_POINTER, INVALID_ARGUMENT, BAD_INPUT};
typedef enum errors error;

typedef struct _Num {
	unsigned int base;
  unsigned int* bodyInt;
  unsigned int lenInt;
  unsigned int* bodyFrac;
  unsigned int lenFrac;
} Num;

int main() {
	return OK;
}
