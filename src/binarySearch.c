#include <stdio.h>
#include "binarySearch.h"

int compare(const void *a, const void *b) {
  int val_a = *(int *)a;
  int val_b = *(int *)b;
  return (val_a - val_b); // Returns >0, 0, o <0
}

void *my_bsearch(const void *key, const void *base, size_t num, size_t width, 
                 int (*cmp)(const void *, const void *)) {

  if (num == 0) return NULL;

  size_t middle = num / 2;
  void *mid_ptr = (char *)base + (middle * width);

  int res = cmp(key, mid_ptr);

  if (res == 0) {
    return mid_ptr;
  } else if (res < 0) {
    return my_bsearch(key, base, middle, width, cmp);
  } else {
    void *right_side = (char *)mid_ptr + width;
    return my_bsearch(key, right_side, num - middle - 1, width, cmp);
  }
}

