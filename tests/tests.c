#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

typedef struct { char *start; size_t size; } heap_chunk;
typedef struct { size_t count; heap_chunk chunks[1024]; } Chunk_List;

extern Chunk_List allocated_chunks;
extern Chunk_List freed_chunks;
void *heap_alloc(size_t size);
void heap_free(void *ptr);
void chunk_list_dump(const char *name, const Chunk_List *list);

void print_banner(const char *title) {
  printf("\n==================================================\n");
  printf(" TEST CASE: %s\n", title);
  printf("==================================================\n");
}


void test_coalescing_verbose() {
  print_banner("EAGER COALESCING (FRAGMENTATION CLEANUP)");

  printf("[STEP 1] Performing 3 contiguous allocations (100 bytes each)...\n");
  void *p1 = heap_alloc(100);
  void *p2 = heap_alloc(100);
  void *p3 = heap_alloc(100);

  //chunk_list_dump("ALLOCATED", &allocated_chunks);
  printf("  -> Free list count: %zu\n", freed_chunks.count);

  printf("\n[STEP 2] Freeing the middle block (p2: %p)...\n", p2);
  heap_free(p2);
  printf("  -> Current free chunks: %zu (Expected: 2 - the hole + the tail)\n", freed_chunks.count);

  printf("\n[STEP 3] Freeing p1 and p3 to trigger coalescing...\n");
  heap_free(p1);
  heap_free(p3);

  printf("  -> Final free chunks: %zu\n", freed_chunks.count);
  if (freed_chunks.count == 1) {
    printf("  [RESULT] SUCCESS: All blocks merged back into 1 large chunk.\n");
  } else {
    printf("  [RESULT] FAILED: Blocks remain fragmented!\n");
  }
  assert(freed_chunks.count == 1);
}

void test_reuse_verbose() {
  print_banner("MEMORY REUSE (HOLE FILLING)");

  printf("[STEP 1] Allocating and freeing to create a specific hole...\n");
  void *p1 = heap_alloc(128);
  void *p2 = heap_alloc(128);
  heap_free(p1);

  printf("[STEP 2] Requesting 64 bytes (should fit in p1's old spot)...\n");
  void *p3 = heap_alloc(64);

  printf("  -> Original p1 address: %p\n", p1);
  printf("  -> New p3 address:      %p\n", p3);

  if (p1 == p3) {
    printf("  [RESULT] SUCCESS: Allocator reused the first available hole.\n");
  }
  assert(p1 == p3);

  heap_free(p2);
  heap_free(p3);
}

void test_stress_random() {
    print_banner("STRESS TEST: 500 RANDOM ALLOCATIONS");
    void *local_ptrs[500] = {0};
    
    printf("[STEP 1] Allocating 500 blocks of varying sizes...\n");
    for (int i = 0; i < 500; i++) {
        size_t size = (i % 64) + 1; 
        local_ptrs[i] = heap_alloc(size);
        assert(local_ptrs[i] != NULL);
    }

    printf("[STEP 2] Freeing all blocks to check for total coalescing...\n");
    for (int i = 0; i < 500; i++) {
        heap_free(local_ptrs[i]);
    }

    printf("  -> Final free chunks: %zu\n", freed_chunks.count);
    printf("  -> Allocated chunks: %zu\n", allocated_chunks.count);
    
    assert(freed_chunks.count == 1);
    assert(allocated_chunks.count == 0);
    printf("  [RESULT] SUCCESS: Memory leaked: 0 bytes. Fragmentation: 0%%.\n");
}

int main() {
  printf("STARTING HEAP ALLOCATOR TEST SUITE\n");
  printf("Heap Capacity: %d bytes\n", 640000);

  test_coalescing_verbose();
  test_reuse_verbose();
  test_stress_random();

  printf("\n==================================================\n");
  printf(" ALL TESTS PASSED SUCCESSFULLY\n");
  printf("==================================================\n");

  return 0;
}
