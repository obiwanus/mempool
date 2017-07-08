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

  Mempool_Element *AllocateElement();
  void FreeElement(Mempool_Element *element);

 private:
  unsigned int chunk_size_;         // size of chunk, in bytes
  unsigned int element_size_;       // size of element, in bytes
  unsigned int per_chunk_;          // number of elements per chunk
  unsigned int total_used_ = 0;     // number of elements in use

  Mempool_Chunk *chunks_ = nullptr;
  Mempool_Chunk *last_chunk_ = nullptr;
  Mempool_Element *free_element = nullptr;

  Mempool_Element *AddChunk(Mempool_Chunk *chunk,
                            Mempool_Element *last_element);
  Mempool_Element *FirstElement(Mempool_Chunk *chunk);
};

#endif  // _MEMPOOL_H_
