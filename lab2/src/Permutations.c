#include <stdlib.h>
#include <stdio.h>
#include "Permutations.h"

error swap(unsigned char *a, unsigned char *b) {
  if (a == NULL || b == NULL)
    return NULL_POINTER;
  *a ^= *b;
  *b ^= *a;
  *a ^= *b;
  return OK;
}

error revert_array(unsigned char * const begin, unsigned char * const end) {
  if (begin == NULL || end == NULL)
    return NULL_POINTER;
  for (unsigned char *i = begin, *j = end; i < j; ++i, --j)
    swap(i, j);
  return OK;
}

error permutate_to_next(Sequence *seq) {
  if (seq == NULL)
    return NULL_POINTER;
  if (seq->length < 2)
    return INVALID_ARGUMENT;

  // Find not increasing suffix:
  unsigned int suffixBorder;
  for (suffixBorder = seq->length - 1; seq->body[suffixBorder - 1] >= seq->body[suffixBorder]; --suffixBorder) {
    if (suffixBorder == 1) // Case when last permutation have already reached
      return INVALID_ARGUMENT;
  }

  unsigned int turnPoint = suffixBorder - 1;

  // Find num to swap with turnPoint:
  unsigned int lastBigger;
  for (lastBigger = seq->length - 1; seq->body[lastBigger] <= seq->body[turnPoint]; --lastBigger);
  error statusSwap = swap(seq->body + turnPoint, seq->body + lastBigger);
  if (statusSwap != OK)
    return statusSwap;

  // Revert suffix:
  error revertStatus = revert_array(seq->body + suffixBorder, seq->body + seq->length - 1);
  return revertStatus;
}

error print_next_permutations(FILE *fout, Sequence const start, unsigned int const num) {
  Sequence seq = start;
  for (unsigned int i = 0; i < num; ++i) {
    error const permutateStatus = permutate_to_next(&seq);
    if (permutateStatus != OK)
      return permutateStatus;

    error const printStatus = print_sequence(fout, seq);
    if (printStatus != OK)
      return printStatus;
  }
  return OK;
}
