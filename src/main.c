#include <stdio.h>
////#include <stdlib.h> //*bsearch()
#include <stdbool.h>
#include <assert.h>
#include "binarySearch.h"

#define HEAP_CAPACITY 640000
#define CHUNK_LIST_CAPACITY 1024

#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

char heap[HEAP_CAPACITY] = {0};

typedef struct
{
  char *start;
  size_t size;
} heap_chunk;
heap_chunk heap_allocated_chunks[CHUNK_LIST_CAPACITY]={0};
size_t heap_allocated_chunks_size = 0;

heap_chunk heap_freed[CHUNK_LIST_CAPACITY] ={0};
size_t heap_freed_size = 0;

typedef struct{
  size_t count;
  heap_chunk chunks[CHUNK_LIST_CAPACITY];

}Chunk_List;
Chunk_List allocated_chunks = {0};
Chunk_List freed_chunks = {
  .count=1,
  .chunks ={
    [0] = {.start=heap, .size = sizeof(heap)}
  },
};
Chunk_List tmp_chunks={0};

void chunk_list_insert(Chunk_List *list, void *start, size_t size){
  assert(list->count < CHUNK_LIST_CAPACITY);
  //insert at end
  list->chunks[list->count].start = start;
  list->chunks[list->count].size = size;
  (void) start;

  for (size_t i=list->count; i>0 && list->chunks[i].start < list->chunks[i-1].start; --i){
    heap_chunk t = list->chunks[i];
    list->chunks[i] = list->chunks[i-1];
    list->chunks[i-1] = t;

  }
  list->count += 1;
  

}


/*  Not sorted list
void chunk_list_merge( Chunk_List *dst, const Chunk_List *src){
  dst->count=0;

  for(size_t i=0; i<src->count; ++i){
    const heap_chunk chunk = src->chunks[i];

    if (dst->count>0){
      heap_chunk *top_chunk = &dst->chunks[dst->count-1];
      if(top_chunk->start + top_chunk->size == chunk.start){
        top_chunk->size += chunk.size;
      } else{
        chunk_list_insert(dst, chunk.start, chunk.size);
      }
    } else{
      chunk_list_insert(dst, chunk.start, chunk.size);
    }
  }
}
*/

//it is already sorted
void chunk_list_merge(Chunk_List *dst, const Chunk_List *src) {
    dst->count = 0;
    if (src->count == 0) return;

    dst->chunks[0] = src->chunks[0];
    dst->count = 1;

    for (size_t i = 1; i < src->count; ++i) {
        heap_chunk *top = &dst->chunks[dst->count - 1];
        heap_chunk current = src->chunks[i];

        if (top->start + top->size == current.start) {
            top->size += current.size;
        } else {
            assert(dst->count < CHUNK_LIST_CAPACITY);
            dst->chunks[dst->count] = current;
            dst->count++;
        }
    }
}

void chunk_list_dump(const  Chunk_List *list){
  printf("Chunks (%zu):\n",list->count);
  for (size_t i=0; i< list->count; ++i){
    printf(" start: %p, size: %zu\n",
           list->chunks[i].start,
           list->chunks[i].size);
  }
}

int chunk_start_comp(const void *a, const void *b){
  
  const heap_chunk *a_chunk = a;
  const heap_chunk *b_chunk = b;
  return a_chunk->start -b_chunk->start;
}


int chunk_list_find(const Chunk_List *list, void *ptr){

  /*  Linear list find
  for(size_t i=0; i<list->count; ++i){
    if (list->chunks[i].start == ptr){
      return (int)i;
    }
  }
  return -1;
  */
  heap_chunk key = {
    .start = ptr
  };

   heap_chunk *result = my_bsearch(&key, list->chunks, list->count,
                 sizeof(list->chunks[0]),
                chunk_start_comp);
  
  if (result !=0){
    assert(list->chunks <= result);
   return (int)(result - list->chunks); 
  }else{
    return -1;
  }


  return -1;
}
void chunk_list_remove(Chunk_List *list, size_t index){
  assert(index < list->count);
  for (size_t i = index; i<list->count-1; ++i){
    list->chunks[i] = list->chunks[i+1];
  }
  list->count -= 1;

}


void *heap_alloc(size_t size){

  if (size > 0){
    // uncomment this for round up size
    //size = ALIGN(size);
    chunk_list_merge(&tmp_chunks, &freed_chunks);
    freed_chunks = tmp_chunks;
  for (size_t i=0; i<freed_chunks.count; ++i){
    const heap_chunk chunk = freed_chunks.chunks[i];
    if (chunk.size >= size){
      chunk_list_remove(&freed_chunks, i);

      const size_t tail_size = chunk.size - size;
      chunk_list_insert(&allocated_chunks, chunk.start, size);

      if(tail_size >0){
        chunk_list_insert(&freed_chunks, chunk.start+size, tail_size);
      }
      return chunk.start;
    }
  }
  }
  return NULL;
}

void chunk_list_coalesce(Chunk_List *list) {
    if (list->count <= 1) return;

    size_t new_count = 1;
    for (size_t i = 1; i < list->count; ++i) {
        heap_chunk *prev = &list->chunks[new_count - 1];
        heap_chunk *curr = &list->chunks[i];

        if (prev->start + prev->size == curr->start) {
            prev->size += curr->size;
        } else {
            list->chunks[new_count] = *curr;
            new_count++;
        }
    }
    list->count = new_count;
}
void heap_free(void *ptr){

  if (ptr != NULL){

    const int index = chunk_list_find(&allocated_chunks, ptr);
    assert(index >= 0);
    assert(ptr == allocated_chunks.chunks[index].start);
    
    heap_chunk chunk = allocated_chunks.chunks[index];
    chunk_list_remove(&allocated_chunks, (size_t)index);
    chunk_list_insert(&freed_chunks, chunk.start, chunk.size);

    /*
    chunk_list_insert(&freed_chunks, allocated_chunks.chunks[index].start,
                      allocated_chunks.chunks[index].size);
    chunk_list_remove(&allocated_chunks, (size_t) index);
    */
    chunk_list_coalesce(&freed_chunks);
  }
  else{
    printf("Freeing a NULL?\n");
  }
}

void heap_collect(void *ptr){

  (void) ptr;
  assert(false && "collect not implemented");

}


#define N 10

void *ptrs[N] = {0};

int main(){
  

  for (int i=0; i<N; ++i){
    ptrs[i] = heap_alloc(i);
  }

  for (int i=0; i<N; ++i){
    if (i%2==0){
      heap_free(ptrs[i]);
    }
  }

  heap_alloc(8);


  printf("Allocated ");
  chunk_list_dump(&allocated_chunks);
  printf("Freed ");
  chunk_list_dump(&freed_chunks);



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

  //heap_free(root);

  return 0;

}
