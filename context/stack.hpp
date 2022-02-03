#pragma once

#include <wheels/memory/view.hpp>
#include <wheels/memory/mmap_allocation.hpp>

#include <cstdint>

namespace context {

class Stack {
 public:
  Stack() = default;

  static Stack AllocatePages(size_t count);

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
