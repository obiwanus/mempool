#include <stdio.h>
#include <time.h>

#define MEMPOOL_DEBUG
#include "mempool.cc"

double time_between(clock_t begin, clock_t end) {
  return (double)(end - begin) / CLOCKS_PER_SEC;
}

int main() {
  Mempool pool(64, 256, 100);
  clock_t begin, end;
  double time_spent;

  const int kObjectCount = 50000000;

  // Test C++ new/delete allocation
  Mempool_Element **elements = new Mempool_Element *[kObjectCount];
  begin = clock();
  for (int i = 0; i < kObjectCount; ++i) {
    elements[i] = new Mempool_Element();
  }
  end = clock();
  time_spent = time_between(begin, end);
  printf("C++ new: allocated in %.2f seconds\n", time_spent);

  // Test pool
  begin = clock();
  for (int i = 0; i < kObjectCount; ++i) {
    Mempool_Element *el = pool.AllocateElement();
  }
  end = clock();
  time_spent = time_between(begin, end);
  printf("Pool: allocated in %.2f seconds...\n", time_spent);

  begin = clock();
  for (int i = 0; i < kObjectCount; ++i) {
    delete elements[i];
  }
  end = clock();
  time_spent = time_between(begin, end);
  printf("C++ delete: freed in %.2f seconds\n", time_spent);

  begin = clock();
  pool.Destroy();
  end = clock();
  time_spent = time_between(begin, end);
  printf("Pool: freed in %.2f seconds\n", time_spent);

}
