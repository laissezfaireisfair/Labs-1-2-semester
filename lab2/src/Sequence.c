#include <stdlib.h>
#include <stdio.h>
#include "Sequence.h"

Sequence make_seq() {
  Sequence seq;
  seq.body = NULL;
  seq.length = 0;
  seq.capacity = 0;
  return seq;
}

error init_seq(Sequence *seq, unsigned int capReq) {
  if (seq == NULL)
    return NULL_POINTER;
  if (capReq == 0)
    return LENGTH_ERROR;
  unsigned char *body = (unsigned char*)malloc(sizeof(unsigned char)*capReq);
  if (body == NULL)
    return RUNTIME_ERROR;
  seq->body = body;
  seq->capacity = capReq;
  seq->length = 0;
  return OK;
}

void deinit_seq(Sequence *seq) {
  if (seq == NULL)
    exit(NULL_POINTER);
  if (seq->body == NULL)
    exit(INVALID_ARGUMENT);
  free(seq->body);
}

error ask_sequence(FILE* fin, Sequence *out, unsigned int const maxLen) {
  if (fin == NULL || out == NULL)
    return NULL_POINTER;
  if (maxLen == 0)
    return LENGTH_ERROR;

  error const initStatus = init_seq(out, maxLen);
  if (initStatus != OK)
    return initStatus;

  for (; out->length < out->capacity; ++out->length) {
    char symbol;
    int const statusRead = fscanf(fin, "%c", &symbol);
    if (statusRead == EOF || symbol == '\n' || symbol == '\r')
      return OK;

    if (statusRead == 0) {
      deinit_seq(out);
      return RUNTIME_ERROR;
    }
    if (symbol > '9' || symbol < '0') {
      deinit_seq(out);
      return BAD_INPUT;
    }

    out->body[out->length] = symbol - '0';
  }

  deinit_seq(out);
  return BAD_INPUT;
 }

error print_sequence(FILE* fout, Sequence const seq) {
  if (fout == NULL)
    return NULL_POINTER;

  for (unsigned int i = 0; i < seq.length; ++i)
    fprintf(fout, "%c", seq.body[i] + '0');

  fprintf(fout, "\n");
  return OK;
}
