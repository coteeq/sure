#pragma once

#include <cstdint>

namespace sure {

// Opaque exceptions context
struct ExceptionsContext {
  // https://itanium-cxx-abi.github.io/cxx-abi/abi-eh.html
  uintptr_t exceptions_state_buf_[2];
};

void SwitchExceptionsContext(ExceptionsContext& from, ExceptionsContext& to);

}  // namespace sure
