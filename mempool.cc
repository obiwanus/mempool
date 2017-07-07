#include "mempool.h"

Mempool::Mempool(unsigned int element_size, unsigned int per_chunk,
                 unsigned int alloc_total) {
  element_size_ = element_size;
  per_chunk_ = per_chunk;
  total_used_ = 0;

  // How many chunks do we need now
  int chunks_to_allocate = 1;
  if (alloc_total > per_chunk) {
    chunks_to_allocate = (alloc_total / per_chunk) + 1;
  }



}

int main() {}
