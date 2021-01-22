#include <context/context.hpp>

#include <wheels/support/compiler.hpp>

#include <cstdint>
#include <cstring>
#include <stdexcept>

namespace context {

//////////////////////////////////////////////////////////////////////

#if __has_feature(address_sanitizer)
#pragma message("Annotate context switch for Address Sanitizer")
#endif

#if __has_feature(thread_sanitizer)
#pragma message("Annotate context switch for Thread Sanitizer")
#endif

//////////////////////////////////////////////////////////////////////

namespace __cxxabiv1 {  // NOLINT

struct __cxa_eh_globals;  // NOLINT

// NOLINTNEXTLINE
extern "C" __cxa_eh_globals* __cxa_get_globals() noexcept;

}  // namespace __cxxabiv1

static void SwitchExceptionsContext(ExecutionContext& from,
                                    ExecutionContext& to) {
  static const size_t kStateSize = 16;

  auto* this_thread_exceptions = __cxxabiv1::__cxa_get_globals();
  memcpy(from.exceptions_state_buf_, this_thread_exceptions, kStateSize);
  memcpy(this_thread_exceptions, to.exceptions_state_buf_, kStateSize);
}

//////////////////////////////////////////////////////////////////////

ExecutionContext::ExecutionContext() {
}

void ExecutionContext::Setup(MemSpan stack, Trampoline trampoline) {
  machine_ctx_.Setup(stack, trampoline);

#if __has_feature(address_sanitizer)
  stack_ = stack.Data();
  stack_size_ = stack.Size();
#endif

#if __has_feature(thread_sanitizer)
  hold_fiber_ = true;
  fiber_ = __tsan_create_fiber(0);
#endif
}

ExecutionContext::~ExecutionContext() {
#if __has_feature(thread_sanitizer)
  if (hold_fiber_) {
    __tsan_destroy_fiber(fiber_);
  }
#endif
}

static thread_local ExecutionContext* from = nullptr;

// NB: `SwitchTo` operates on 3 (!) contexts: this, target, to
// SwitchTo: this -> target -> ... -> from -> this

void ExecutionContext::SwitchTo(ExecutionContext& target) {
  // Prepare this->target switch

  from = this;

  SwitchExceptionsContext(*this, target);

#if __has_feature(address_sanitizer)
  void* fake_stack;
  __sanitizer_start_switch_fiber(&fake_stack, target.stack_,
                                 target.stack_size_);
#endif

#if __has_feature(thread_sanitizer)
  fiber_ = __tsan_get_current_fiber();
  __tsan_switch_to_fiber(target.fiber_, 0);
#endif

  // Switch stacks
  machine_ctx_.SwitchTo(target.machine_ctx_);

  // Finalize from->this switch

#if __has_feature(address_sanitizer)
  __sanitizer_finish_switch_fiber(fake_stack, &(from->stack_),
                                  &(from->stack_size_));
#endif
}

void ExecutionContext::AfterStart() {
#if __has_feature(address_sanitizer)
  __sanitizer_finish_switch_fiber(nullptr, &(from->stack_),
                                  &(from->stack_size_));
#endif
}

void ExecutionContext::ExitTo(ExecutionContext& target) {
  from = this;

  SwitchExceptionsContext(*this, target);

#if __has_feature(address_sanitizer)
  __sanitizer_start_switch_fiber(nullptr, target.stack_, target.stack_size_);
#endif

#if __has_feature(thread_sanitizer)
  __tsan_switch_to_fiber(target.fiber_, 0);
#endif

  machine_ctx_.SwitchTo(target.machine_ctx_);

  WHEELS_UNREACHABLE();
}

}  // namespace context
