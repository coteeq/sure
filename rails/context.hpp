#pragma once

#include <rails/trampoline.hpp>
#include <rails/machine_context.hpp>
#include <rails/exceptions.hpp>

#include <wheels/memory/view.hpp>

#include <cstdlib>
#include <cstdint>

namespace rails {

// Execution Context =
// 1) Machine context (registers) +
// 2) [Address | Thread] sanitizer context +
// 3) Exceptions context

class ExecutionContext : public ITrampoline {
 public:
  // Empty context, cannot be a target for SwitchTo
  ExecutionContext();

  // Non-copyable
  ExecutionContext(const ExecutionContext&) = delete;
  ExecutionContext& operator=(const ExecutionContext&) = delete;

  // Non-movable
  ExecutionContext(ExecutionContext&&) = delete;
  ExecutionContext& operator=(ExecutionContext&&) = delete;

  ~ExecutionContext();

  // Prepare execution context for running trampoline->Run()
  // on stack `stack`
  void Setup(wheels::MutableMemView stack, ITrampoline* trampoline);

  // Symmetric Control Transfer
  // 1) Save current execution context to `this`
  // 2) Activate `target` context
  void SwitchTo(ExecutionContext& target);

  // Leave current execution context forever
  // Last context switch in ITrampoline::Run
  [[noreturn]] void ExitTo(ExecutionContext& target);

 private:
  // ITrampoline
  void Run() override;

  // Finalize first context switch
  void AfterStart();

 private:
  ITrampoline* user_trampoline_;

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

#if defined(RAILS_CAPTURE_EXCEPTIONS_CONTEXT)
  // 3) Exceptions
  ExceptionsContext exceptions_ctx_;
#endif
};

}  // namespace rails
