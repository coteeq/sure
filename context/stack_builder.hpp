#pragma once

#include <cstdint>

namespace context {

class StackBuilder {
  using Self = StackBuilder;

  using Word = std::uintptr_t;
  static const size_t kWordSize = sizeof(Word);

 public:
  StackBuilder(char* bottom) : top_(bottom) {
  }

  void AlignNextPush(size_t alignment) {
    size_t shift = (size_t)(top_ - kWordSize) % alignment;
    top_ -= shift;
  }

  void* Top() const {
    return top_;
  }

  Self& Push(Word value) {
    top_ -= kWordSize;
    *(Word*)top_ = value;
    return *this;
  }

  Self& Allocate(size_t bytes) {
    top_ -= bytes;
    return *this;
  }

 private:
  char* top_;
};

}  // namespace context