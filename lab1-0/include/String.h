#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "Errors.h"

typedef struct _String {
  unsigned char* body;
  unsigned int length;
  unsigned int capacity;
} String;

String make_str();

error init_str(String *str, unsigned int capReq);

void deinit_str(String *str);

error add_symbol(char const symbol, String *string);

error ask_string(FILE* fin, String *out, unsigned int const maxLen);

error ask_text(FILE* fin, String *out, unsigned int const maxLen);

error print_string(FILE* fout, String const seq);
