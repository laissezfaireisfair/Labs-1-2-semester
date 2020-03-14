#pragma once
#include "Errors.h"

typedef struct _String {
  unsigned char* body;
  unsigned int length;
  unsigned int capacity;
} String;

String make_str();

error init_str(String *str, unsigned int capacityReq);

error deinit_str(Strgin *str);
