#pragma once
#include "Errors.h"

typedef struct _Sequence {
  int* body;
  unsigned int length;
  unsigned int capacity;
} Sequence;

Sequence make_seq();

error init_seq(Sequence *seq, unsigned int capReq);

error deinit_seq(Sequence *seq);

error ask_sequence(FILE* fin, Sequence *out, unsigned int const maxLen)
