#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

char const TRUE = 1;
char const FALSE = 0;
unsigned int const STR_CAPACITY = 256;

typedef struct _String {
  unsigned char* body;
  unsigned int length;
  unsigned int capacity;
} String;

String ask_String(FILE* fin) { //TODO: Make it memory-friendly and not limited
  assert(fin != NULL);
  String str;
  str.capacity = STR_CAPACITY;
  str.body = (unsigned char*)malloc(sizeof(unsigned char) * str.capacity);
  assert(str.body != NULL);
  str.length = 0;
  for (; str.length < str.capacity; ++str.length) {
    char symbol;
    if (fscanf(fin, "%c", &symbol) == EOF) {
      str.body[str.length] = 0;
      break;
    }
    str.body[str.length] = symbol;
  }
  return str;
 }

void find_substr(String const substr, String const str, FILE* fout) {
  unsigned int const maxNumOfSymbols = 256;
  assert(fout != NULL);
  assert(substr.length != 0);
  assert(str.length != 0);
  assert(substr.length <= str.length);
  int stopTable[maxNumOfSymbols]; // Distance from begin to first symbol position
  for (unsigned int i = 0; i < maxNumOfSymbols; ++i)
    stopTable[i] = -1;
  for (unsigned int i = 0; i < substr.length; ++i)
    stopTable[substr.body[substr.length - i - 1]] = substr.length - i - 1;
  for (unsigned int i = substr.length - 1, j = i, last = i; i < str.length;) {
    if (last < i)
      last = i; // Last checked symbol
    fprintf(fout, "%u ", i + 1);
    if (substr.body[j] == str.body[i]) {
      if (j == 0) {
        i = last + substr.length;
        j = substr.length - 1;
        continue;
      }
      --j;
      --i;
      continue;
    }
    if (stopTable[str.body[last]] != -1)
      i = last + substr.length - stopTable[str.body[last]] - 1;
    else
      i = last + substr.length;
    j = substr.length - 1;
  }
}

int main(void) {
  FILE* fin = fopen("in.txt", "r");
  String substr = ask_String(fin);
  String str = ask_String(fin);
  fclose(fin);
  FILE* fout = fopen("out.txt", "w");
  find_substr(substr, str, fout);
  fprintf(fout, "\n");
  fclose(fout);
  free(substr.body);
  free(str.body);
  return 0;
}
