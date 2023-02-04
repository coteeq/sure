#pragma once

#include <wheels/memory/view.hpp>
#include <wheels/system/mmap.hpp>

#include <cstdint>

namespace sure {

// Stack = MmapAllocation with guard page

class Stack {
 public:
  Stack() = default;

  // Deprecated
  static Stack AllocatePages(size_t count);

  static Stack AllocateBytes(size_t at_least);

  Stack(Stack&& that) = default;
  Stack& operator=(Stack&& that) = default;

  size_t AllocationSize() const {
    return allocation_.Size();
  }

  wheels::MutableMemView View() {
    return allocation_.View();
  }

 private:
  Stack(wheels::MmapAllocation allocation);

 private:
  wheels::MmapAllocation allocation_;
};

}  // namespace sure
