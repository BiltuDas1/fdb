#include <iostream>
// #include <rocksdb/db.h>

namespace Process {
  char *read(const char *operand, long size, size_t *resp_size) {
    char *r = new char[2];
    r[0] = 0;
    *resp_size = 1;
    return r;
  }

  char *write(const char *operand, long size, size_t *resp_size) {
    char *r = new char[2];
    r[0] = 6;
    r[1] = 1;
    *resp_size = 2;
    return r;
  }
}