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
    last_element = AddChunk(NewChunk(), last_element);
  }
}

/**
 * Returns a free element from the pool. If there are none,
 * allocates a new chunk
 */
Mempool_Element *Mempool::AllocateElement() {
  Mempool_Element *element;

  if (free_element_ == nullptr) {
    // Need to allocate a new chunk
    Mempool_Chunk *chunk = NewChunk();
    AddChunk(chunk);
  }

  // Get the next element from the free list
  element = free_element_;
  free_element_ = element->next_free;

  total_used_++;

  return element;
}

/**
 * Frees an element from the pool by adding it to the free list
 */
void Mempool::FreeElement(Mempool_Element *element) {
  assert(total_used_ > 0);

#ifdef MEMPOOL_DEBUG
  {
    // Check the element being freed does indeed belong to the pool
    Mempool_Chunk *chunk;
    bool found = false;
    intptr_t element_address = (intptr_t)element;
    for (chunk = chunks_; chunk != nullptr; chunk = chunk->next) {
      intptr_t chunk_start = (intptr_t)chunk;
      intptr_t chunk_end = chunk_start + chunk_size_;
      if (chunk_start < element_address && element_address < chunk_end) {
        found = true;
        break;
      }
    }
    if (!found) {
      assert(!"Freeing an element that is not in the pool");
    }
  }
#endif  // MEMPOOL_DEBUG

  // Append the element to the head of the free list
  element->next_free = free_element_;
  free_element_ = element;

  total_used_--;
}

/**
 * Allocates a new chunk
 */
Mempool_Chunk *Mempool::NewChunk() {
  return (Mempool_Chunk *)malloc(chunk_size_);
}

/**
 * Adds an allocated chunk to the pool
 * last_chunk_tail: last element of the previous chunk (used when adding
                    chunks to pool in a loop, so that the last element of
                    a chunk can point to the first element of the next chunk)
 */
Mempool_Element *Mempool::AddChunk(Mempool_Chunk *chunk,
                                   Mempool_Element *last_chunk_tail) {
  Mempool_Element *element = GetFirstElement(chunk);

  assert(free_element_ == nullptr || last_chunk_tail == nullptr);

  if (chunks_tail_) {
    // Append the chunk to the pool linked list
    chunks_tail_->next = chunk;
  } else {
    // We're adding the first chunk to the pool
    assert(chunks_ == nullptr);
    chunks_ = chunk;
  }
  chunks_tail_ = chunk;

  // If this is a newly created pool, or the pool ran out of free elements,
  // add the first element to the free list
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
    element->next_free = GetNextElement(element);
    element = element->next_free;
  }
  element->next_free = nullptr;  // terminate the list

  return element;
}

/**
 * Returns a pointer to the area of memory immediately following
 * the chunk head (and that is where the first element of the chunk is)
 */
Mempool_Element *Mempool::GetFirstElement(Mempool_Chunk *chunk) {
  return (Mempool_Element *)(chunk + 1);
}

/**
 * Returns a pointer to where the next adjacent element should be located
 */
Mempool_Element *Mempool::GetNextElement(Mempool_Element *element) {
  return (Mempool_Element *)((char *)element + element_size_);
}

int main() {}
