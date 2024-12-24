#include <iostream>
#include <cstdlib>
#include <stack>
#include <mutex>
#include <unordered_map>

namespace Pool {
  class MemoryPool
  {
  private:
    size_t maxPool;
    size_t totalAllocatedMemory = 0;
    void *pool;
    void *start_block;
    void *end_block;
    std::unordered_map<size_t, std::stack<void*>> freeBlocks;
    std::mutex lck;

  public:
    MemoryPool(size_t totalPoolsize) {
      maxPool = totalPoolsize;
      pool = std::malloc(totalPoolsize);
      if (pool == nullptr)
      {
        throw std::bad_alloc();
      }
      start_block = pool;
      end_block = pool;
    }

    /**
     * @brief Allocate Memory from the Memory Pool
     * @param size Size of the memory block which will be allocated (in bytes)
     * @param fragmentation (Default is true), if true then this method will try
     * to allocated memory from already deallocated memory blocks, otherwise it will allocate
     * memory sequentially
     * 
     * 
     * This function allocates some memory from the pool, which can be reused multiple times. 
     * It allows to dynamically allocate memory blocks without struggling the computer to find
     * the next free memory locations. Also it prevents fragmentation (As much as possible).
     * 
     * 
     * @returns
     * Void pointer which consist the starting address of the allocated memory
     */
    void *allocate(size_t size, bool fragmentation = true) {
      std::lock_guard<std::mutex> lock(this->lck); // Lock

      // Checks if allocated memory is higher than the pool
      if ((this->getAllocatedSize() + size) > maxPool)
      {
        throw std::bad_alloc();
      }

      // Checks if memory can be allocated from freeMemory blocks
      if (!freeBlocks.empty() && fragmentation) {
        std::size_t matched = 0;
        std::size_t actualSize = size;
        while (matched < freeBlocks.size())
        {
          auto iter = freeBlocks.find(size);

          // If needed memory found
          if (iter != freeBlocks.end() && iter->second.size() != 0) {
            ++matched;
            void *choosedAdd = iter->second.top();
            iter->second.pop();

            // If stack is empty then remove the key from the map
            if (iter->second.size() == 0) {
              freeBlocks.erase(size);
            }

            // Check if the allocated size is the same as the actual Required Size
            // If not then add unused memory to the freeBlocks List
            if (size > actualSize) {
              int fragmentMem = size - actualSize;
              void *fragAdd = (char*)choosedAdd + fragmentMem;
              freeBlocks[fragmentMem].push(fragAdd);
            }

            totalAllocatedMemory += actualSize;
            return choosedAdd;
          }
          ++size;
        }
      }


      // Do the new allocation
      void *top_block = end_block;
      end_block = (char *)end_block + size;
      totalAllocatedMemory += size;
      return top_block;
    }

    /**
     * @brief Deallocate the whole memory
     */
    void deallocate() {
      std::lock_guard<std::mutex> lock(this->lck); // Lock
      end_block = start_block;
      this->totalAllocatedMemory = 0;
    }

    /**
     * @brief Deallocate the custom memory
     * @param *c Pointer to the address of the memory which is going to be free
     * @param size The size of the memory block (in bytes)
     * 
     * The function allows to deallocate specific portion of the memory which 
     * is passed to the function
     * Note this doesn't free the memory, rather than it store the address of
     * current memory location and then return to the user for reuse.
     */
    void deallocate(void *c, size_t size) {
      std::lock_guard<std::mutex> lock(this->lck); // Lock
      freeBlocks[size].push(c);
      this->totalAllocatedMemory -= size;
    }

    // Get the pool size
    inline size_t size() const {
      return this->maxPool;
    }

    // Get the total used memory size
    inline size_t getAllocatedSize() const {
      return this->totalAllocatedMemory;
    }

    // Deallocate Memory Pool
    ~MemoryPool() {
      std::free(pool);
    }
  };
}