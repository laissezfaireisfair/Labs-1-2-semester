#include <stdlib.h>
#include <stdin.h>

char* ask_string(FILE* fin);

int find_substr(char* const substr, char* const str, FILE* fout);

int main(void) {
  FILE* fin = fopen("in.txt", "r");
  char *substr = ask_string(fin);
  char *str = ask_string(fin);
  fclose(fin);
  if (substr == NULL || str == NULL)
    return 1;
  FILE* fout = fopen("out.txt", "w");
  int const status = find_substr(substr, str, fout);
  fclose(fout);
  if (status != 0)
    return 2;
  return 0;
}
