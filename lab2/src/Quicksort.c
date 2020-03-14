#include <stdlib.h>
#include "Errors.h"

error swap(int *a, int *b) {
  if (a == NULL || b == NULL)
    return NULL_POINTER;
  *a ^= *b;
  *b ^= *a;
  *a ^= *b;
  return OK;
}

error sort(int* arr, unsigned int const len) {
  if (arr == NULL)
    return NULL_POINTER;
  if (len < 2)
    return OK;
  int const midVal = arr[len / 2];
  int left = 0, right = len - 1;
  while (left <= right) {
    for (; arr[left] < midVal; ++left);
    for (; arr[right] > midVal; --right);
    if (left <= right) {
      if (arr[left] > arr[right]) {
        error swapStatus = swap(arr+left, arr+right);
        if (swapStatus != OK)
          return swapStatus;
      }
      ++left;
      --right;
    }
  }
  return sort(arr, right+1);
  return sort(arr + left, len - left);
}
