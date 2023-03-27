#pragma once

#include <wheels/memory/view.hpp>
#include <wheels/system/mmap.hpp>

#include <cstdint>

namespace sure {

// Stack = mmap allocation with guard page

class Stack {
 public:
  Stack() = delete;

  static Stack AllocateBytes(size_t at_least);

  // Allocated memory will be released to the operating system
  ~Stack() = default;

  Stack(Stack&&) = default;
  Stack& operator=(Stack&&) = default;

  // Including guard page
  size_t AllocationSize() const {
    return allocation_.Size();
  }

  wheels::MutableMemView MutView();

  // Release / acquire ownership for the underlying memory region
  [[nodiscard("Memory leak")]] wheels::MutableMemView Release();
  static Stack Acquire(wheels::MutableMemView view);

 private:
  Stack(wheels::MmapAllocation allocation);

  static Stack AllocatePages(size_t count);

 private:
  wheels::MmapAllocation allocation_;
};

}  // namespace sure
