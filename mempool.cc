#include <stdlib.h>
#include <assert.h>

#include "mempool.h"

// TODO: add function comments

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

Mempool_Element *Mempool::AddChunk(Mempool_Chunk *chunk,
                                   Mempool_Element *last_element) {
  Mempool_Element *tail = nullptr;
  // TODO:

  return tail;
}

/**
 * Returns a pointer to the area of memory immediately following
 * the chunk head (and that is where the first element of the chunk is)
 */
Mempool_Element *Mempool::FirstElement(Mempool_Chunk *chunk) {
  return (Mempool_Element *)(chunk + 1);
}

int main() {}
