#include <stdio.h>
#include <stdbool.h>
#include <assert.h>


#define HEAP_CAPACITY 640000
#define HEAP_ALLOCATED_CHUNKS_CAPACITY 1024
#define HEAP_FREED_CAPACITY 1024

char heap[HEAP_CAPACITY] = {0};
size_t heap_size = 0;

typedef struct
{
  void *start;
  size_t size;
} heap_chunk;
heap_chunk heap_allocated_chunks[HEAP_ALLOCATED_CHUNKS_CAPACITY]={0};
size_t heap_allocated_chunks_size = 0;

heap_chunk heap_freed[HEAP_FREED_CAPACITY] ={0};
size_t heap_freed_size = 0;

typedef struct{
  size_t count;
  heap_chunk chunks[HEAP_ALLOCATED_CHUNKS_CAPACITY];

}Chunk_List;

int chunk_list_find(const Chunk_List *list, void *ptr){
  assert(false && "not implemented");
  return -1;

}

void chunk_list_insert(Chunk_List *list, void *ptr, size_t size){
  assert(false && "not implemented");
  return -1;
}

void chunk_list_remove(Chunk_List *list, size_t index){
  assert(false && "not implemented");
  return -1;
}

Chunk_List allocated_chunks = {0};
Chunk_List freed_chunks = {0};


void *heap_alloc(size_t size){

  if (size <= 0)
    return NULL;

  assert(heap_size + size <= HEAP_CAPACITY);
  void *ptr = heap + heap_size;
  heap_size += size;
  chunk_list_insert(&allocated_chunks, ptr);
  return ptr;
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
  // Iterate all existing chunks
  for (size_t i=0; i<heap_allocated_chunks_size; ++i){
    if (heap_allocated_chunks[i].start == ptr){
         
    }
  }
  
}

void heap_collect(void *ptr){

  (void) ptr;
  assert(false && "collect not implemented");

}


int main(){
 
  for (int i=0; i<100; i++){
    void *p = heap_alloc(i);
    if (i%2 == 0){
      heap_free(p);
    }
  }



/*
  for (int i=0; i<100; i++){
    heap_alloc(i);
  }
  

  char *root = heap_alloc(26);
  for (int i=0; i<26; i++){
    root[i] = i+'A';
  }

  for (int i=0; i<26; i++){
    printf("%c", root[i]);
  }
*/
  heap_dump_allocated_chunks();

  //heap_free(root);

  return 0;

}
