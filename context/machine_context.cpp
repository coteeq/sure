#include <context/machine_context.hpp>

#include <context/stack_builder.hpp>

namespace context {

//////////////////////////////////////////////////////////////////////

// Switch between MachineContext-s
extern "C" void SwitchMachineContext(void* from_rsp, void* to_rsp);

//////////////////////////////////////////////////////////////////////

// View for stack-saved machine context
struct StackSavedMachineContext {
  // Layout of the StackSavedMachineContext matches the layout of the stack
  // in context.S at the 'Switch stacks' comment

  // Callee-saved registers
  // Saved manually in SwitchMachineContext
  void* rbp;
  void* rbx;

  void* r12;
  void* r13;
  void* r14;
  void* r15;

  // Saved automatically by 'call' instruction
  void* rip;
};

static void* SetupStack(StackView stack, Trampoline trampoline) {
  // https://eli.thegreenplace.net/2011/02/04/where-the-top-of-the-stack-is-on-x86/

  StackBuilder builder(stack.Back());

  // Ensure trampoline will get 16-byte aligned frame pointer (rbp)
  // 'Next' here means first 'pushq %rbp' in trampoline prologue
  builder.AlignNextPush(16);

  // Reserve space for stack-saved context
  builder.Allocate(sizeof(StackSavedMachineContext));

  auto* saved_context = (StackSavedMachineContext*)builder.Top();
  saved_context->rip = (void*)trampoline;

  return saved_context;
}

void MachineContext::Setup(StackView stack, Trampoline trampoline) {
  rsp_ = SetupStack(stack, trampoline);
}

void MachineContext::SwitchTo(MachineContext& target) {
  SwitchMachineContext(&rsp_, &target.rsp_);
}

}  // namespace context
