#include <sure/context.hpp>

#include <wheels/core/compiler.hpp>

#include <cstdint>
#include <cstring>

namespace sure {

//////////////////////////////////////////////////////////////////////

ExecutionContext::ExecutionContext() {
}

void ExecutionContext::Setup(wheels::MutableMemView stack, ITrampoline* trampoline) {
  user_trampoline_ = trampoline;
  machine_ctx_.Setup(stack, this);
  sanitizer_ctx_.Setup(stack);
}

void ExecutionContext::SwitchTo(ExecutionContext& target) {
#if defined(SURE_CAPTURE_EXCEPTIONS_CONTEXT)
  SwitchExceptionsContext(exceptions_ctx_, target.exceptions_ctx_);
#endif

  sanitizer_ctx_.BeforeSwitch(target.sanitizer_ctx_);

  // Switch stacks
  machine_ctx_.SwitchTo(target.machine_ctx_);

  sanitizer_ctx_.AfterSwitch();
}

void ExecutionContext::ExitTo(ExecutionContext& target) {
#if defined(SURE_CAPTURE_EXCEPTIONS_CONTEXT)
  SwitchExceptionsContext(exceptions_ctx_, target.exceptions_ctx_);
#endif

  sanitizer_ctx_.BeforeExit(target.sanitizer_ctx_);

  // Switch stacks
  machine_ctx_.SwitchTo(target.machine_ctx_);

  WHEELS_UNREACHABLE();
}


void ExecutionContext::AfterStart() {
  sanitizer_ctx_.AfterStart();
}

void ExecutionContext::Run() noexcept {
  AfterStart();
  user_trampoline_->Run();
}

}  // namespace sure
