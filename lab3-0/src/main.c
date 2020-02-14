#include <stdlib.h>
#include <stdio.h>

int* ask_array(int* lenptr) {
  FILE *fin = fopen("in.txt", "r");
  if (!fscanf(fin, "%d", lenptr))
    return NULL;
  int *arr = (int*)malloc(sizeof(int)*(*lenptr));
  for (int i = 0; i < *lenptr; ++i)
    if (!fscanf(fin, "%d", arr+i)) {
      free(arr);
      fclose(fin);
      return NULL;
    }
  fclose(fin);
  return arr;
}

void swap(int *a, int *b) {
  *a ^= *b;
  *b ^= *a;
  *a ^= *b;
}

void sort(int* arr, int const len) {
  if (len < 2)
    return;
  int const midVal = arr[len / 2];
  int left = 0, right = len - 1;
  while (left <= right) {
    for (; arr[left] < midVal; ++left);
    for (; arr[right] > midVal; --right);
    if (left <= right) {
      if (arr[left] > arr[right])
        swap(arr+left, arr+right);
      ++left;
      --right;
    }
  }
  sort(arr, right+1);
  sort(arr + left, len - left);
}

void print_array(int* const arr, int const len) {
  FILE *fout = fopen("out.txt", "w");
  for (int i = 0; i < len; ++i)
    fprintf(fout, "%d ", arr[i]);
  fclose(fout);
}

int main(void) {
  int len;
  int* arr = ask_array(&len);
  if (arr == NULL)
    return 1;
  sort(arr, len);
  print_array(arr, len);
  free(arr);
  return EXIT_SUCCESS;
}
