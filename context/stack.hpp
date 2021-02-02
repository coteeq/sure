#pragma once

#include <wheels/support/memspan.hpp>
#include <wheels/support/mmap_allocation.hpp>

#include <cstdint>

namespace context {

using wheels::MemSpan;
using wheels::MmapAllocation;

class Stack {
 public:
  Stack() = default;

  static Stack AllocatePages(size_t count);

  // Backward compatibility
  static Stack Allocate(size_t pages) {
    return AllocatePages(/*count=*/pages);
  }

  Stack(Stack&& that) = default;
  Stack& operator=(Stack&& that) = default;

  char* Bottom() const;

  size_t Size() const {
    return allocation_.Size();
  }

  // With guard page!
  MemSpan AsMemSpan() const;

 private:
  Stack(MmapAllocation allocation);

 private:
  MmapAllocation allocation_;
};

}  // namespace context
