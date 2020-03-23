#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "Errors.h"

typedef struct _Sequence {
  unsigned char* body;
  unsigned int length;
  unsigned int capacity;
} Sequence;

Sequence make_seq();

error init_seq(Sequence *seq, unsigned int capReq);

void deinit_seq(Sequence *seq);

error ask_sequence(FILE* fin, Sequence *out, unsigned int const maxLen);

error print_sequence(FILE* fout, Sequence const seq);
