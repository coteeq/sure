#include <rails/context.hpp>

#include <sanitizer/asan_interface.h>
#include <sanitizer/tsan_interface.h>

#include <wheels/support/compiler.hpp>

#include <cstdint>
#include <cstring>

namespace rails {

//////////////////////////////////////////////////////////////////////

#if __has_feature(address_sanitizer)
#pragma message("Annotate context switch for Address Sanitizer")
#endif

#if __has_feature(thread_sanitizer)
#pragma message("Annotate context switch for Thread Sanitizer")
#endif

//////////////////////////////////////////////////////////////////////

ExecutionContext::ExecutionContext() {
}

void ExecutionContext::Setup(wheels::MutableMemView stack, ITrampoline* trampoline) {
  user_trampoline_ = trampoline;
  machine_ctx_.Setup(stack, this);

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

#if __has_feature(address_sanitizer)
static thread_local ExecutionContext* last = nullptr;
#endif

// NB: `SwitchTo` operates on 3 (!) contexts: this, target, last
// SwitchTo: this -> target -> ... -> last -> this

void ExecutionContext::SwitchTo(ExecutionContext& target) {
  // Prepare this -> target switch

#if __has_feature(address_sanitizer)
  last = this;
#endif

#if defined(exceptions_context)
  SwitchExceptionsContext(exceptions_ctx_, target.exceptions_ctx_);
#endif

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

  // Finalize last -> this switch

#if __has_feature(address_sanitizer)
  __sanitizer_finish_switch_fiber(fake_stack, &(last->stack_),
                                  &(last->stack_size_));
#endif
}

void ExecutionContext::ExitTo(ExecutionContext& target) {
#if __has_feature(address_sanitizer)
  last = this;
#endif

#if defined(exceptions_context)
  SwitchExceptionsContext(exceptions_ctx_, target.exceptions_ctx_);
#endif

#if __has_feature(address_sanitizer)
  // https://github.com/llvm-mirror/compiler-rt/blob/69445f095c22aac2388f939bedebf224a6efcdaf/include/sanitizer/common_interface_defs.h#L299
  __sanitizer_start_switch_fiber(nullptr, target.stack_, target.stack_size_);
#endif

#if __has_feature(thread_sanitizer)
  __tsan_switch_to_fiber(target.fiber_, 0);
#endif

  // Switch stacks
  machine_ctx_.SwitchTo(target.machine_ctx_);

  WHEELS_UNREACHABLE();
}


void ExecutionContext::AfterStart() {
#if __has_feature(address_sanitizer)
  __sanitizer_finish_switch_fiber(nullptr, &(last->stack_),
                                  &(last->stack_size_));
#endif
}

void ExecutionContext::Run() {
  AfterStart();
  user_trampoline_->Run();
}

}  // namespace rails