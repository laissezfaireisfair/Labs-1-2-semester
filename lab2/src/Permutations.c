#include <stdlib.h>
#include <stdio.h>
#include "Quicksort.h"
#include "Permutations.h"

error print_next_permutations(FILE *fout, Sequence const start, unsigned int const num) {
  Sequence seq = start;
  for (unsigned int i = 0; i < num; ++i) {
    error const permutateStatus = permutate_to_next(seq);
    if (permutateStatus != OK)
      return permutateStatus;

    error const printStatus = print_sequence(seq);;
    if (printStatus != OK)
      return printStatus;
  }
  return OK;
}
