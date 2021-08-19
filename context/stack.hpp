#pragma once

#include <wheels/support/mem_view.hpp>
#include <wheels/support/mmap_allocation.hpp>

#include <cstdint>

namespace context {

class Stack {
 public:
  Stack() = default;

  static Stack AllocatePages(size_t count);

  [[deprecated("Use AllocatePages instead")]]
  static Stack Allocate(size_t pages) {
    return AllocatePages(/*count=*/pages);
  }

  Stack(Stack&& that) = default;
  Stack& operator=(Stack&& that) = default;

  char* Bottom() const;

  size_t Size() const {
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

}  // namespace context
