#include <stdlib.h>
#include <stdin.h>

struct string {
  char* body;
  unsigned int length;
  unsigned int const capacity = 256;
};

/// Returns pointer to stt if OK, otherwise - error code
string ask_string(FILE* fin) { //TODO: Make it memory-friendly and not limited
  string str;
  str.body = (char*)malloc(sizeof(char) * str.capacity);
  length = 0;
  do {
    char symbol;
    fscanf(fin, "%c", &symbol);
    if (symbol == '\n')
      str.body[str.length] = 0;
    else {
      str.body[str.length] = symbol;
      ++str.length;
    }
  } while(symbol != '\n' && str.length < str.capacity);
  return str;
}

/// Returns 1 if substr, 0 - not substr, otherwise - error code
int is_substr_here(string const substr, string const str, int const pos);


/// Returns 1 if founded, 0 if not founded, otherwise - error code
int find_substr(string const substr, string const str, FILE* fout) {
  int const numOfSymbols = 256;
  if (substr.length > str.length)
    return 2;
  int stopTable[numOfSymbols];
  for (int i = 0; i < numOfSymbols; ++i)
    stopTable[i] = 0;
  for (int i = substr.length - 1; i >= 0; --i)
    stopTable[substr.body[i]] = i;
  for (int i = substr.length() - 1, j = substr.length - 1; i < str.length;) {
    char const symbFromStr = str.body[i];
    char const symbFromSubstr = substr.body[j];
    if (symbFromStr == symbFromSubstr) {
      const int beginPerhapsPos = i - stopTable[symbFromStr];
      const int checkStatus = is_substr_here(substr, str, beginPerhapsPos);
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
      j = stopTable[symbFromSubstr];
  }
  return 0;
}

int main(void) {
  FILE* fin = fopen("in.txt", "r");
  string substr = ask_string(fin);
  if (substr.body == NULL) {
    fclose(fin);
    return 1;
  }
  string str = ask_string(fin);
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
  if (status != 0)
    return 3;
  return 0;
}
