#include <stdlib.h>
#include <stdio.h>
#include "Errors.h"
#include "Sequence.h"
#include "Permutations.h"

int main() {
  int const          MAX_SEQ_LEN      = 10;
  char const * const INPUT_FILE_NAME  = "in.txt";
  char const * const OUTPUT_FILE_NAME = "out.txt";

  Sequence seq = make_seq();

  FILE *fin = fopen(INPUT_FILE_NAME, "r");
  if (fin == NULL) {
    print_error(OUTPUT_FILE_NAME, RUNTIME_ERROR);
    return RUNTIME_ERROR;
  }

  error const readStatus = ask_sequence(fin, &seq, MAX_SEQ_LEN);
  if (readStatus != OK) {
    print_error(OUTPUT_FILE_NAME, readStatus);
    return readStatus;
  }

  unsigned int num;
  if (fscanf(fin, "%u", num) == 0) {
    print_error(OUTPUT_FILE_NAME, BAD_INPUT);
    return BAD_INPUT;
  }
  fclose(fin);

  FILE *fout = fopen(OUTPUT_FILE_NAME, "r");
  if (fout == NULL) {
    print_error("error.txt", RUNTIME_ERROR);
    return RUNTIME_ERROR;
  }

  error const printStatus = print_next_permutations(fout, seq, num);
  if (printStatus != OK) {
    print_error(OUTPUT_FILE_NAME, printStatus);
    return printStatus;
  }
  fclose(fout);

  return OK;
}
