#include <stdio.h>
#include <stdbool.h>
#include <assert.h>



#define HEAP_CAPACITY 640000

char heap[HEAP_CAPACITY] = {0};
size_t heap_size = 0;

void *heap_alloc(size_t size){
  assert(heap_size + size <= HEAP_CAPACITY);
  void *result = heap + heap_size;
  heap_size += size;

  return result;
}

void heap_free(void *ptr){
  (void) ptr;
  assert(false && "free not implemented");

}

void heap_collect(void *ptr){

  (void) ptr;
  assert(false && "collect not implemented");

}

int main(){
  char *root = heap_alloc(26);
  for (int i=0; i<26; i++){
    root[i] = i+'A';
  }

  for (int i=0; i<26; i++){
    printf("%c", root[i]);
  }
  return 0;

}
