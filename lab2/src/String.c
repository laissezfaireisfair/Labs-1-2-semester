#include "String.h"

String make_str() {
  String str;
  str.body = NULL;
  str.length = 0;
  str.capacity = 0;
  return str;
}

error init_str(String *str, unsigned int capacityReq) {
  if (str == NULL)
    return NULL_POINTER;
  if (capacityReq == 0)
    return LENGTH_ERROR;
  unsigned char *body = (unsigned char*)malloc(sizeof(unsigned char)*capacityReq);
  if (body == NULL)
    return RUNTIME_ERROR;
  str->body = body;
  str->capacity = capacityReq;
  str->length = 0;
  return OK;
}

error deinit_str(Strgin *str) {
  if (str == NULL)
    return NULL_POINTER;
  if (str.body == NULL)
    return INVALID_ARGUMENT;
  free(str->body);
  return OK;
}
