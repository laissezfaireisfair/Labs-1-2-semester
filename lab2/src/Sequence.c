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

error deinit_seq(seqgin *seq) {
  if (seq == NULL)
    return NULL_POINTER;
  if (seq.body == NULL)
    return INVALID_ARGUMENT;
  free(seq->body);
  return OK;
}
