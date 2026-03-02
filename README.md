# Custom Heap Allocator in C

## Overview

Custom implementation of a memory management system in C. It replaces the standard malloc and free functions with a manually managed heap. The system manages a static 640KB buffer and uses a metadata-driven approach to track memory segments.

## System Architecture

1. **Memory Buffer**  
   The heap is simulated using a global char array of 640,000 bytes. This ensures that all memory operations are confined to a specific, pre-allocated segment of the data section, preventing interference with the system's actual stack or heap.

2. **Metadata Management: The Chunk List**  
   The allocator tracks memory using two distinct lists:

   - **Allocated List**: Tracks blocks currently in use by the user.
   - **Free List**: Tracks available "holes" in the heap that can be used for future requests.

   Each "chunk" is defined by a starting pointer and a size. These lists are kept sorted by memory address to facilitate efficient searching and merging.

## Core Memory Logic

1. **8-Byte Alignment**  
   To ensure compatibility with CPU architectures (e.g., ARM, x86_64) and prevent alignment faults, the allocator can enforce a strict 8-byte alignment policy. Every requested size is rounded up to the nearest multiple of 8 using the formula:

    aligned_size = (size + 7) & ~7

   This ensures that every returned pointer satisfies:

     ptr % 8 == 0

2. **Allocation Strategy (First-Fit with Splitting)**  
   The system employs a First-Fit strategy. When `heap_alloc` is called:

   1. The Free List is scanned.
   2. The first block with chunk.size≥requested_size is selected.
   3. **Splitting**: If the selected block is significantly larger than requested, it is split. The requested portion moves to the Allocated List, and the remaining "tail" is re-inserted into the Free List.

3. **Deallocation and Eager Coalescing**  
   When `heap_free` is called, the system must return the block to the Free List without creating unnecessary fragmentation.

   - **Binary Search Lookup**: To find the block in the Allocated List, the system uses a binary search algorithm, reducing lookup time to $O(\log n)$.
   - **Eager Coalescing**: Once a block is moved back to the Free List, the allocator immediately checks its physical neighbors. If the end of Chunk A is exactly the start of Chunk B, they are merged into a single contiguous block. This prevents "External Fragmentation," where many small free blocks exist but none are large enough for a new request.

## Testing and Validation

The project includes a dedicated unit testing suite (`tests/tests.c`) to verify the stability of the allocator.

### How to Run Tests

To compile and execute the test suite, use the provided Makefile:

```bash
make test
```

**Included Test Cases:**
- **Eager Coalescing**: Makes sure the merge of contigous freed blocks can be done.
- **Hole Re-use**: Ensures that once a block is freed, the allocator can fill that exact hole with a new request.
- **Stress Test**: Performs 500 randomized allocations and deallocations. The test is successful only if the heap returns to a single, contiguous free chunk (0% fragmentation) at the end of the cycle.

## Known Problems and Limitations

1. **Fixed Metadata Capacity**  
   The system tracks chunks using a fixed-size array (`CHUNK_LIST_CAPACITY = 1024`). If the user performs more than 1024 concurrent allocations, the system will fail to track new blocks, even if there is physical space available in the 640KB buffer.

2. **Metadata Insertion Overhead**  
   While lookups are $O(\log n)$, inserting or removing elements from the metadata lists requires shifting elements in an array, which is an $O(n)$ operation. In scenarios with tens of thousands of small allocations, this overhead becomes significant.

3. **Thread Safety**  
   The allocator is not thread-safe. There are no mutexes or atomic guards protecting the global Chunk_List structures. Simultaneous calls from multiple threads will cause race conditions and heap corruption.

## Possible Improvements

- **Best-Fit Strategy**: Implement a Best-Fit algorithm to choose the smallest available hole that fits a request, further reducing internal fragmentation.
- **Dynamic Metadata**: Replace the fixed-size array with a linked list or a dynamic array that grows as the number of allocations increases.
- **Segregated Free Lists**: Group free blocks by size to speed up the search for large blocks.
- **Realloc Implementation**: Add a `heap_realloc` function that can expand or shrink existing allocations in place if neighboring memory is free.
- **Garbage Collection**: Implement a `heap_collect` function using a Mark-and-Sweep algorithm to reclaim memory from orphaned pointers.
