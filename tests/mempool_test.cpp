#include "catch2/catch_amalgamated.hpp"
#include "../include/core/mempool.hpp"

TEST_CASE("FixedMemPool Allocation integers", "[fixedmempool]") {
  auto *pool = new Pool::FixedMemoryPool<int>(sizeof(int) * 3);
  int *a = pool->allocate();
  int *b = pool->allocate();
  int *c = pool->allocate();
  *a = 10;
  *b = 20;
  *c = 30;
  REQUIRE(*a == 10);
  REQUIRE(*b == 20);
  REQUIRE(*c == 30);
}

TEST_CASE("FixedMemPool Deallocation integers", "[fixedmempool]") {
  auto *pool = new Pool::FixedMemoryPool<int>(sizeof(int) * 3);
  int *a = pool->allocate();
  int *b = pool->allocate();
  int *c = pool->allocate();
  *a = 10;
  *b = 20;
  *c = 30;
  pool->deallocate(b);
  pool->deallocate(a);
  pool->deallocate(c);
  int *d = pool->allocate();
  int *e = pool->allocate();
  int *f = pool->allocate();
  *d = 100;
  *e = 200;
  *f = 300;
  REQUIRE(*d == 100);
  REQUIRE(*e == 200);
  REQUIRE(*f == 300);
}

TEST_CASE("FixedMemPool Overflow", "[fixedmempool]") {
  auto *pool = new Pool::FixedMemoryPool<int>(sizeof(int));
  int *a = pool->allocate();
  *a = 10;
  REQUIRE_THROWS_AS(pool->allocate(), std::length_error);
}

TEST_CASE("Check If memory reused", "[fixedmempool]") {
  auto *pool = new Pool::FixedMemoryPool<int>(sizeof(int));
  int *a = pool->allocate();
  *a = 30;
  pool->deallocate(a);
  REQUIRE(*a == 30); // Cause Memory Pool never clear the address, it rather than reuse it
  int *b = pool->allocate();
  *b = 40;
  REQUIRE(*a == 40);
  REQUIRE(*a == *b);
}

TEST_CASE("Check if memory reuse can disable", "[fixedmempool]") {
  auto *pool = new Pool::FixedMemoryPool<int>(sizeof(int) * 3);
  int *a = pool->allocate();
  *a = 4;
  int *b = pool->allocate();
  *b = 5;
  pool->deallocate(a);
  int *c = pool->allocate(false);
  *c = 7;

  REQUIRE(*a != *c);
}