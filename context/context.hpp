#pragma once

#include <context/machine_context.hpp>

#include <sanitizer/asan_interface.h>
#include <sanitizer/tsan_interface.h>

#include <cstdlib>
#include <cstdint>

namespace context {

using wheels::MemSpan;

// Execution Context =
// 1) Machine context (registers) +
// 2) [Address | Thread] sanitizer context +
// 3) Exceptions context

struct ExecutionContext {
  // 1) Machine context (registers)

  MachineContext machine_ctx_;

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

  // Finalize first context switch
  void AfterStart();

  // Leave current context forever
  // Never returns
  void ExitTo(ExecutionContext& target);
};

}  // namespace context
