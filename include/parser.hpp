#include <uv.h>
#include <iostream>
#include <cstring>

namespace Parser {
  // Read the client information and process it
  char *parse(const char *data, ssize_t size, size_t *resp_size) {
    // Requires Dynamic Memory allocation to work properly
    *resp_size = 1;
    char *response = new char[*resp_size];
    *response = data[0];
    return response;
  }
}