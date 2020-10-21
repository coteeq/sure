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

struct ExecutionContext {
  // Execution context saved on top of suspended fiber/thread stack
  void* rsp_;

#if __has_feature(address_sanitizer)
  const void* stack_;
  size_t stack_size_;
#endif

#if __has_feature(thread_sanitizer)
  bool hold_fiber_{false};
  void* fiber_;
#endif

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
