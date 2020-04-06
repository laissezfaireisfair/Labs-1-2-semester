#include <stdlib.h>
#include <stdio.h>
#include "Errors.h"
#include "String.h"

error find_substr(String const substr, String const str, FILE* fout) {
  if (fout == NULL)
    return NULL_POINTER;
  if (substr.length == 0 || str.length == 0)
    return INVALID_ARGUMENT;

  // Distance from begin to first symbol position:
  int *stopTable = (int*)malloc(sizeof(int) * substr.length);
  for (unsigned int i = 0; i < substr.length; ++i)
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

  free(stopTable);
  return OK;
}

int main(void) {
  unsigned int const maxStrLen = 256;
  char const *inputFilename    = "input.txt";
  char const *outputFilename   = "output.txt";

  FILE* fin = fopen(inputFilename, "r");
  if (fin == NULL) {
    print_error(outputFilename, RUNTIME_ERROR);
    return 1;
  }

  String substr = make_str(), str = make_str();
  error const statusReadStr = ask_string(fin, &substr, maxStrLen);
  if (statusReadStr != OK) {
    print_error(outputFilename, statusReadStr);
    return 2;
  }
  error const statusReadTxt = ask_text(fin, &str, maxStrLen);
  fclose(fin);
  if (statusReadTxt != OK) {
    print_error(outputFilename, statusReadTxt);
    deinit_str(&substr);
    return 3;
  }

  FILE* fout = fopen(outputFilename, "w");
  if (fout == NULL) {
    print_error(outputFilename, RUNTIME_ERROR);
    deinit_str(&substr);
    deinit_str(&str);
    return 4;
  }
  error const findStatus = find_substr(substr, str, fout);
  if (findStatus == OK)
    fprintf(fout, "\n");
  else
    print_error(outputFilename, findStatus);

  deinit_str(&substr);
  deinit_str(&str);
  fclose(fout);
  free(substr.body);
  free(str.body);
  return 0;
}
