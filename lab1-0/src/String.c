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

error add_symbol(char const symbol, String *string) {
  if (string == NULL)
    return NULL_POINTER;
  if (string->length == string->capacity)
    return LENGTH_ERROR;

  string->body[string->length] = symbol;
  ++string->length;
  return OK;
}

error ask_string(FILE* fin, String *out, unsigned int const maxLen) {
  if (fin == NULL || out == NULL)
    return NULL_POINTER;
  if (maxLen == 0)
    return LENGTH_ERROR;

  error const initStatus = init_str(out, maxLen);
  if (initStatus != OK)
    return initStatus;

  while (out->length < out->capacity) {
    char symbol;
    int const statusRead = fscanf(fin, "%c", &symbol);
    if (statusRead == EOF || symbol == '\n' || symbol == '\r')
      return OK;

    if (statusRead == 0) {
      deinit_str(out);
      return RUNTIME_ERROR;
    }

    error const statusAdd = add_symbol(symbol, out);
    if (statusAdd != OK)
      return statusAdd;
  }

  deinit_str(out);
  return LENGTH_ERROR;
 }

 error ask_text(FILE* fin, String *out, unsigned int const maxLen) {
   if (fin == NULL || out == NULL)
     return NULL_POINTER;
   if (maxLen == 0)
     return LENGTH_ERROR;

   error const initStatus = init_str(out, maxLen);
   if (initStatus != OK)
     return initStatus;

   while (out->length < out->capacity) {
     char symbol;
     int const statusRead = fscanf(fin, "%c", &symbol);
     if (statusRead == EOF)
       return OK;

     if (statusRead == 0) {
       deinit_str(out);
       return RUNTIME_ERROR;
     }

     error const statusAdd = add_symbol(symbol, out);
     if (statusAdd != OK)
       return statusAdd;
   }

   deinit_str(out);
   return LENGTH_ERROR;
  }

error print_string(FILE* fout, String const str) {
  if (fout == NULL)
    return NULL_POINTER;

  for (unsigned int i = 0; i < str.length; ++i)
    fprintf(fout, "%c", str.body[i] + '0');

  fprintf(fout, "\n");
  return OK;
}
