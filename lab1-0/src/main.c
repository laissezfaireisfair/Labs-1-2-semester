#include <stdlib.h>
#include <stdin.h>

char* ask_string(FILE* fin) { //TODO: Make it memory-friendly and not limited
  char str[256];
  fscanf(fin, "%s", &str);
  return str;
}

int find_substr(char* const substr, char* const str, FILE* fout) {
  int const numOfSymbols = 256;
  int stopTable[numOfSymbols];
  for (int i = 0; i < numOfSymbols; ++i)
    stopTable[i] = 0;
  for (int i = 0; substr[i] != 0; ++i)
    stopTable[substr[i]] = i + 1;
}

int main(void) {
  FILE* fin = fopen("in.txt", "r");
  char *substr = ask_string(fin);
  if (substr == NULL) {
    fclose(fin);
    return 1;
  }
  char *str = ask_string(fin);
  if (str == NULL) {
    fclose(fin);
    free(substr);
    return 2;
  }
  fclose(fin);
  FILE* fout = fopen("out.txt", "w");
  int const status = find_substr(substr, str, fout);
  fclose(fout);
  free(substr);
  free(str);
  if (status != 0)
    return 3;
  return 0;
}
