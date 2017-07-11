#include <stdio.h>
#include <time.h>

#define MEMPOOL_DEBUG
#include "mempool.cc"

int main() {
  Mempool pool(64, 256, 100);
  clock_t begin, end;
  double time_spent;

  const int kObjectCount = 50000000;

  // Test pool
  begin = clock();
  for (int i = 0; i < kObjectCount; ++i) {
    Mempool_Element *el = pool.AllocateElement();
  }
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Pool: allocated in %.2f seconds...\n", time_spent);

  begin = clock();
  pool.Destroy();
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Pool: freed in %.2f seconds\n", time_spent);

  // Test individual allocation
  Mempool_Element *elements = (Mempool_Element *)malloc(kObjectCount * sizeof(Mempool_Element *));
  for (int i = 0; i < kObjectCount; ++i) {

  }
}
