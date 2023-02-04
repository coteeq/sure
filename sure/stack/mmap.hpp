#pragma once

#include <wheels/memory/view.hpp>
#include <wheels/system/mmap.hpp>

#include <cstdint>

namespace sure {

// Stack = MmapAllocation with guard page

class Stack {
 public:
  Stack() = default;

  static Stack AllocateBytes(size_t at_least);

  Stack(Stack&& that) = default;
  Stack& operator=(Stack&& that) = default;

  size_t AllocationSize() const {
    return allocation_.Size();
  }

  wheels::MutableMemView MutView();

  static Stack Acquire(wheels::MutableMemView view);
  wheels::MutableMemView Release();

 private:
  Stack(wheels::MmapAllocation allocation);

  static Stack AllocatePages(size_t count);

 private:
  wheels::MmapAllocation allocation_;
};

}  // namespace sure
