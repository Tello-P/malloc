#include <stdio.h>
#include <stdbool.h>
#include <assert.h>



#define HEAP_CAPACITY 640000
#define HEAP_ALLOCATED_CHUNKS_CAPACITY 1024


char heap[HEAP_CAPACITY] = {0};
size_t heap_size = 0;

typedef struct
{
  void *start;
  size_t size;
} heap_chunk;
heap_chunk heap_allocated_chunks[HEAP_ALLOCATED_CHUNKS_CAPACITY]={0};
size_t heap_allocated_chunks_size = 0;


void *heap_alloc(size_t size){
  assert(heap_size + size <= HEAP_CAPACITY);
  void *result = heap + heap_size;
  heap_size += size;

  heap_chunk chunk = {
    .start = result,
    .size = size,
  };
  assert(heap_allocated_chunks_size < HEAP_ALLOCATED_CHUNKS_CAPACITY);
  heap_allocated_chunks[heap_allocated_chunks_size++] = chunk;

  return result;
}

void heap_dump_allocated_chunks(void){
  printf("Allocated chunks (%zu):\n", heap_allocated_chunks_size);
  for (size_t i=0; i< heap_allocated_chunks_size; ++i){
    printf(" start: %p, size: %zu\n",
           heap_allocated_chunks[i].start,
           heap_allocated_chunks[i].size);
  }
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
  for (int i=0; i<100; i++){
    heap_alloc(i);
  }


  char *root = heap_alloc(26);
  for (int i=0; i<26; i++){
    root[i] = i+'A';
  }
/*
  for (int i=0; i<26; i++){
    printf("%c", root[i]);
  }
*/
  heap_dump_allocated_chunks();

  //heap_free(root);

  return 0;

}
