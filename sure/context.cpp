#include <sure/context.hpp>

#include <wheels/core/compiler.hpp>

namespace sure {

ExecutionContext::ExecutionContext() {
}

void ExecutionContext::Setup(wheels::MutableMemView stack, ITrampoline* trampoline) {
  user_trampoline_ = trampoline;
  machine_.Setup(stack, this);
  sanitizer_.Setup(stack);
}

void ExecutionContext::SwitchTo(ExecutionContext& target) {
  exceptions_.SwitchTo(target.exceptions_);

  sanitizer_.BeforeSwitch(target.sanitizer_);

  // Switch stacks
  machine_.SwitchTo(target.machine_);

  sanitizer_.AfterSwitch();
}

void ExecutionContext::ExitTo(ExecutionContext& target) {
  exceptions_.SwitchTo(target.exceptions_);

  sanitizer_.BeforeExit(target.sanitizer_);

  // Switch stacks
  machine_.SwitchTo(target.machine_);

  WHEELS_UNREACHABLE();
}

void ExecutionContext::AfterStart() {
  sanitizer_.AfterStart();
}

void ExecutionContext::Run() noexcept {
  AfterStart();
  user_trampoline_->Run();
}

}  // namespace sure
