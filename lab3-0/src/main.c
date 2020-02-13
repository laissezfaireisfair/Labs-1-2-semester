#include <stdlib.h>
#include <stdio.h>

int* ask_array(int* lenptr) {
  FILE *fin = fopen("in.txt", "r");
  fscanf(fin, "%d", lenptr);
  int *arr = (int*)malloc(sizeof(int)*(*lenptr));
  for (int i = 0; i < *lenptr; ++i)
    fscanf(fin, "%d", arr+i);
  fclose(fin);
  return arr;
}

void sort(int* arr, int const len);

void print_array(int* const arr, int const len) {
  FILE *fout = fopen("out.txt", "w");
  for (int i = 0; i < len; ++i)
    fprintf(fout, "%d ", arr[i]);
  fclose(fout);
}

int main(void) {
  int len;
  int* arr = ask_array(&len);
  //sort(arr, len);
  print_array(arr, len);
  free(arr);
  return EXIT_SUCCESS;
}
