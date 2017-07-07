#ifndef _MEMPOOL_H_
#define _MEMPOOL_H_

struct Mempool_Element {
  Mempool_Element *next;
};

struct Mempool_Chunk {
  Mempool_Chunk *next;
};

class Mempool {
 public:
  Mempool(unsigned int element_size, unsigned int per_chunk,
          unsigned int alloc_total);

  Mempool_Element *allocate_element();
  void free_element(Mempool_Element *element);

 private:
  unsigned int element_size_;  // size of element, in bytes
  unsigned int per_chunk_;     // number of elements per chunk
  unsigned int total_used_;    // number of elements in use

  Mempool_Chunk *chunks_;
  Mempool_Chunk *last_chunk_;
};

#endif  // _MEMPOOL_H_
