#pragma once

#include <context/trampoline.hpp>
#include <context/machine_context.hpp>
#include <context/exceptions.hpp>

#include <wheels/memory/view.hpp>

#include <cstdlib>
#include <cstdint>

namespace context {

// Execution Context =
// 1) Machine context (registers) +
// 2) [Address | Thread] sanitizer context +
// 3) Exceptions context

struct ExecutionContext : public ITrampoline {
  // 1) Machine context (registers)
  ITrampoline* user_trampoline_;
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

  // 3) Exceptions
  ExceptionsContext exceptions_ctx_;

  // Empty context, cannot be a target for SwitchTo
  ExecutionContext();

  // Non-copyable
  ExecutionContext(const ExecutionContext&) = delete;
  ExecutionContext& operator=(const ExecutionContext&) = delete;

  // Non-movable
  ExecutionContext(ExecutionContext&&) = delete;
  ExecutionContext& operator=(ExecutionContext&&) = delete;

  ~ExecutionContext();

  // Prepare execution context for running ITrampoline::Run
  void Setup(wheels::MutableMemView stack, ITrampoline* trampoline);

  // Symmetric Control Transfer
  // 1) Save the current execution context to 'this'
  // 2) Activate 'target' context
  void SwitchTo(ExecutionContext& target);

  // Use in trampoline:

  // Leave current context forever
  // Never returns
  [[deprecated]]
  void ExitTo(ExecutionContext& target);

 private:
  // ITrampoline
  void Run() override;

  // Finalize first context switch
  void AfterStart();
};

}  // namespace context
