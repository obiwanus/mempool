#include <stdlib.h>
#include <assert.h>

#include "mempool.h"

/**
 * Creates a memory pool
 * element_size: the size of the element data (excluding the header)
 * per_chunk: number of elements per chunk
 * alloc_total: total number of elements to allocate (used to calculate
 *              how many chunks we need right now to store them)
 */
Mempool::Mempool(unsigned int element_size, unsigned int per_chunk,
                 unsigned int alloc_total) {
  // Calculate sizes of chunk and element including the headers
  per_chunk_ = per_chunk;
  element_size_ = sizeof(Mempool_Element) + element_size;
  chunk_size_ = sizeof(Mempool_Chunk) + element_size_ * per_chunk;
  total_used_ = 0;

  // How many chunks do we need to allocate now
  int chunks = 1;
  if (alloc_total > per_chunk) {
    chunks = (alloc_total / per_chunk) + 1;
  }

  // Allocate actual chunks and add them to the pool
  Mempool_Element *last_element = nullptr;
  for (int i = 0; i < chunks; ++i) {
    Mempool_Chunk *chunk = (Mempool_Chunk *)malloc(chunk_size_);
    last_element = AddChunk(chunk, last_element);
  }
}

/**
 * Adds an allocated chunk to the pool
 * last_chunk_tail: last element of the previous chunk (used when adding
                    chunks to pool in a loop, so that the last element of
                    a chunk can point to the first element of the next chunk)
 */
Mempool_Element *Mempool::AddChunk(Mempool_Chunk *chunk,
                                   Mempool_Element *last_chunk_tail = nullptr) {
  Mempool_Element *element = FirstElement(chunk);

  assert(free_element_ == nullptr || last_chunk_tail == nullptr);

  // If this is a newly created pool, add the first element to the free list
  if (free_element_ == nullptr) {
    free_element_ = element;
  }

  // Point to the first element of the chunk from the last element
  // of the previous chunk, if passed
  if (last_chunk_tail != nullptr) {
    last_chunk_tail->next_free = element;
  }

  // Make the initial free list of elements
  for (int i = 0; i < (per_chunk_ - 1); ++i) {
    element->next_free = NextElement(element);
    element = element->next_free;
  }
  element->next_free = nullptr;  // terminate the list

  return element;
}

/**
 * Returns a pointer to the area of memory immediately following
 * the chunk head (and that is where the first element of the chunk is)
 */
Mempool_Element *Mempool::FirstElement(Mempool_Chunk *chunk) {
  return (Mempool_Element *)(chunk + 1);
}

/**
 * Returns a pointer to where the next adjacent element should be located
 */
Mempool_Element *Mempool::NextElement(Mempool_Element *element) {
  return (Mempool_Element *)((char *)element + element_size_);
}

int main() {}
