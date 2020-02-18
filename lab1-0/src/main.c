#include <stdlib.h>
#include <stdin.h>

struct string {
  char* body;
  unsigned int length;
  unsigned int const capacity = 256;
}

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
      ++length;
    }
  } while(symbol != '\n' && str.length < str.capacity);
  return str;
}

int find_substr(string const substr, string const str, FILE* fout) {
  int const numOfSymbols = 256;
  int stopTable[numOfSymbols];
  for (int i = 0; i < numOfSymbols; ++i)
    stopTable[i] = 0;
  for (int i = 0; substr.body[i] != 0; ++i)
    stopTable[substr.body[i]] = i + 1;
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
