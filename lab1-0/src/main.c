#include <stdlib.h>
#include <stdio.h>

char const TRUE = 1;
char const FALSE = 0;
unsigned int const STR_CAPACITY = 256;

typedef struct _String {
  unsigned char* body;
  unsigned int length;
  unsigned int capacity;
} String;

/// Returns pointer to stt if OK, otherwise - error code
String ask_String(FILE* fin) { //TODO: Make it memory-friendly and not limited
  String str;
  str.capacity = STR_CAPACITY;
  str.body = (unsigned char*)malloc(sizeof(unsigned char) * str.capacity);
  str.length = 0;
  for (int strEnded = FALSE; !strEnded && str.length < str.capacity;) {
    char symbol;
    fscanf(fin, "%c", &symbol);
    if (symbol == '\n') {
      strEnded = TRUE;
      str.body[str.length] = 0;
    } else {
      str.body[str.length] = symbol;
      ++str.length;
    }
  }
  return str;
}

/// Returns 1 if substr, 0 - not substr, otherwise - error code
int is_substr_here(String const substr, String const str, int const pos, FILE* fout) {
  if (str.length == 0 || substr.length == 0)
    return 2;
  if (str.length < substr.length + pos)
    return 0;
  for (int i = substr.length - 1; i >= 0; --i) {
    fprintf(fout, "%d ", i + pos + 1);
    if (substr.body[i] != str.body[i + pos])
      return 0;
  }
  return 1;
}


/// Returns 1 if founded, 0 if not founded, otherwise - error code
int find_substr(String const substr, String const str, FILE* fout) {
  int const numOfSymbols = 256;
  if (substr.length > str.length)
    return 2;
  int stopTable[numOfSymbols];
  for (int i = 0; i < numOfSymbols; ++i)
    stopTable[i] = -1;
  for (int i = substr.length - 1; i >= 0; --i)
    stopTable[substr.body[i]] = i;
  for (unsigned int i = substr.length-1, j = substr.length-1; i < str.length;) {
    unsigned char const symbFromStr = str.body[i];
    unsigned char const symbFromSubstr = substr.body[j];
    fprintf(fout, "%d ", i + 1);
    if (symbFromStr == symbFromSubstr) {
      const int beginPerhapsPos = i - stopTable[symbFromStr];
      const int checkStatus = is_substr_here(substr, str, beginPerhapsPos, fout);
      if (checkStatus == 1)
        return 1;
      if (checkStatus == 0) {
        i += substr.length;
        j = 0;
        continue;
      }
      return checkStatus;
    }
    else
      if (stopTable[symbFromStr] != -1)
        j = stopTable[symbFromStr];
      else {
        i += substr.length;
        j = 0;
      }
  }
  return 0;
}

int main(void) {
  FILE* fin = fopen("in.txt", "r");
  String substr = ask_String(fin);
  if (substr.body == NULL) {
    fclose(fin);
    return 1;
  }
  String str = ask_String(fin);
  if (str.body == NULL) {
    fclose(fin);
    free(substr.body);
    return 2;
  }
  fclose(fin);
  FILE* fout = fopen("out.txt", "w");
  int const status = find_substr(substr, str, fout);
  fclose(fout);
  free(substr.body);
  free(str.body);
  if (status != 1)
    return 3;
  return 0;
}
