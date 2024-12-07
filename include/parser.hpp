#include <uv.h>
#include <iostream>
#include <cstring>
#include "process.hpp"

namespace Parser {
  // Read the client information and process it
  // Returns results in byte array
  // The first position character is the flag,
  // 0 when there is no Data
  // 6 when success
  // 21 when failed
  char *parse(const char *data, ssize_t size, size_t *resp_size) {
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