#ifndef BINARYSEARCH_H
#define BINARYSEARCH_H

#include <stddef.h> // size_t

int compare(const void *a, const void *b);
void *my_bsearch(const void *key, const void *base, size_t num, size_t width, 
                 int (*cmp)(const void *, const void *));

#endif
