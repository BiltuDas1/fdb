#include <iostream>
// #include <rocksdb/db.h>

namespace Process {
  /**
   * @brief Finds for the respective data, depends on the operand
   * 
   * The function takes an fixed size of data bits which is stored in unsigned character
   * and then looks for the data bits into persistent memory, and then give results
   * 
   * @param *operand Array of bytes received from the client (Contains only operand)
   * @param size The total size of the `operand` array
   * @param *resp_size Reference to a mutable variable which stores the size of the output returned by this function
   * 
   * @returns 
   * If successful returns 6 as the first element of the array, and rest is the response data,
   * If failed returns [21] (In bytes)
  **/ 
  char *read(const char *operand, long size, size_t *resp_size) {
    // Read logic Comes here...
  }

  /**
   * @brief Stores the operand into disk
   * 
   * The function takes an fixed size of data bits which is stored in unsigned character
   * and then process and store the bits into Radix Tree, and into RocksDB in a such way
   * that we can later retrieve the data from the secondary memory
   * 
   * @param *operand Array of bytes received from the client (Contains only operand)
   * @param size The total size of the `operand` array
   * @param *resp_size Reference to a mutable variable which stores the size of the output returned by this function
   * 
   * @returns 
   * If successful returns [6] (In bytes),
   * If failed returns [21] (In bytes)
  **/ 
  char *write(const char *operand, long size, size_t *resp_size) {
    // Write Logic Comes here...
  }
}