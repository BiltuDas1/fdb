#include <uv.h>
#include <iostream>
#include <cstring>
#include "process.hpp"

namespace Parser {
  /**
   * @brief Function which parse the data from the client and process it
   * 
   * This function Reads the client information (in bytes) and split the opcode and
   * operand section from it. After reading the opcode the function decides what operation
   * to perform and then call the respective operation function for execution.
   * 
   * The data is passed into this function is an array but the first element of the array (i.e at 0 position)
   * always contains the opcode which tells what operation to perform. The operand section is optional but 
   * the opcode section is mandatory to have. If invalid opcode encounter then the function returns [0].
   * 
   * @param *data Array of bytes received from the client (Contains opcode and operand)
   * @param size The total size of the `data` array
   * @param *resp_size Reference to a mutable variable which stores the size of the output returned by this function
   * 
   * @returns 
   * If successful then it returns Dynamic Array of Chars (Contains bytes instead of valid ascii chars),
   * Otherwise it returns [0] (In bytes)
   **/
  char *parse(const char *data, ssize_t size, size_t *resp_size) noexcept {
    char *resp = nullptr;
    *resp_size = 1;

    switch ((ushort)data[0]) {
      // For Reading the information
      case 10:
        resp = Process::read(data + 1, size-1, resp_size);
        break;

      // For Writing the information
      case 11:
        resp = Process::write(data + 1, size-1, resp_size);
        break;

      default:
        break;
    }

    // Dynamic Memory allocation to work properly
    char *response = new char[*resp_size];
    // If resp variable is empty then
    if (resp == nullptr) {
      response[0] = 0;
      return response;  // No Data
    }

    std::memcpy(response, resp, *resp_size);
    return response;
  }
}