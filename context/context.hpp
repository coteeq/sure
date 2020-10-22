#pragma once

#include <wheels/support/memspan.hpp>

#include <sanitizer/asan_interface.h>
#include <sanitizer/tsan_interface.h>

#include <cstdlib>
#include <cstdint>

namespace context {

using wheels::MemSpan;

// TODO(Lipovsky): closure instead of void(void) function
typedef void (*Trampoline)();

// Execution Context =
// 1) Machine context (registers) +
// 2) [Address | Thread] sanitizer context +
// 3) Exceptions context

struct ExecutionContext {
  // 1) Machine context (registers)

  // Machine context saved on top of the suspended stack
  void* rsp_;

  // 2) Sanitizers context

#if __has_feature(address_sanitizer)
  const void* stack_;
  size_t stack_size_;
#endif

#if __has_feature(thread_sanitizer)
  bool hold_fiber_{false};
  void* fiber_;
#endif

  // 3) Opaque exceptions context
  // https://itanium-cxx-abi.github.io/cxx-abi/abi-eh.html
  uintptr_t exceptions_state_buf_[2];

  // Empty context, cannot be a target for SwitchTo
  ExecutionContext();

  ~ExecutionContext();

  // Prepare execution context for running trampoline function
  void Setup(MemSpan stack, Trampoline trampoline);

  // Save the current execution context to 'this' and jump to the
  // 'target' context. 'target' context created directly by Setup or
  // by another target.SwitchTo(other) call.
  void SwitchTo(ExecutionContext& target);

  // Use in trampoline:
  void AfterStart();
  void Return(ExecutionContext& target);
};

}  // namespace context
