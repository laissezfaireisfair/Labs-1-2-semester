#pragma once
#include "Sequence.h"

error permutate_to_next(Sequence *seq);

error print_next_permutations(FILE *fout, Sequence const start, unsigned int const num);
