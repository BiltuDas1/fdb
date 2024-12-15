#include <iostream>
#include <cstdlib>

namespace Pool {
  class MemoryPool {
    private:
      size_t maxPool;
      void *pool;

    public:
      void* start_block;
      void* end_block;
      MemoryPool(size_t totalPoolsize) {
        maxPool = totalPoolsize;
        pool = std::malloc(totalPoolsize);
        if (pool == nullptr) {
          throw std::bad_alloc();
        }
        start_block = pool;
        end_block = pool;
      }

      // Giving Memory
      void* allocate(size_t size) {
        void *top_block = end_block;
        end_block = (char *)end_block + size;
        return top_block;
      }

      // Deallocating Memory
      void deallocate(){
        end_block = start_block;
      }

      // Get the pool size
      size_t size() {
        return maxPool;
      }

      // Deallocate Memory Pool
      ~MemoryPool() {
        std::free(pool);
      }
  };
}
