#include <stdlib.h>
#include <stdio.h>
#include "String.h"

String make_str() {
  String str;
  str.body = NULL;
  str.length = 0;
  str.capacity = 0;
  return str;
}

error init_str(String *str, unsigned int capReq) {
  if (str == NULL)
    return NULL_POINTER;
  if (capReq == 0)
    return LENGTH_ERROR;
  unsigned char *body = (unsigned char*)malloc(sizeof(unsigned char)*capReq);
  if (body == NULL)
    return RUNTIME_ERROR;
  str->body = body;
  str->capacity = capReq;
  str->length = 0;
  return OK;
}

void deinit_str(String *str) {
  if (str == NULL)
    exit(NULL_POINTER);
  if (str->body == NULL)
    exit(INVALID_ARGUMENT);
  free(str->body);
}

error ask_string(FILE* fin, String *out, unsigned int const maxLen) {
  if (fin == NULL || out == NULL)
    return NULL_POINTER;
  if (maxLen == 0)
    return LENGTH_ERROR;

  error const initStatus = init_str(out, maxLen);
  if (initStatus != OK)
    return initStatus;

  for (; out->length < out->capacity; ++out->length) {
    char symbol;
    int const statusRead = fscanf(fin, "%c", &symbol);
    if (statusRead == EOF || symbol == '\n' || symbol == '\r')
      return OK;

    if (statusRead == 0) {
      deinit_str(out);
      return RUNTIME_ERROR;
    }

    out->body[out->length] = symbol;
  }

  deinit_str(out);
  return BAD_INPUT;
 }

error print_string(FILE* fout, String const str) {
  if (fout == NULL)
    return NULL_POINTER;

  for (unsigned int i = 0; i < str.length; ++i)
    fprintf(fout, "%c", str.body[i] + '0');

  fprintf(fout, "\n");
  return OK;
}
