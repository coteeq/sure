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
  // in machine_context.S at the 'Switch stacks' comment

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

// https://eli.thegreenplace.net/2011/09/06/stack-frame-layout-on-x86-64/
static void MachineContextTrampoline(void*, void*, void*, void*, void*, void*, void* arg1, void* arg2) {
  Trampoline t = (Trampoline)arg1;
  t(arg2);
}

static void* SetupStack(wheels::MutableMemView stack, Trampoline trampoline, void* arg) {
  // https://eli.thegreenplace.net/2011/02/04/where-the-top-of-the-stack-is-on-x86/

  StackBuilder builder(stack.Back());

  // Preallocate space for arguments
  builder.Allocate(sizeof(uintptr_t) * 3);

  // Ensure trampoline will get 16-byte aligned frame pointer (rbp)
  // 'Next' here means first 'pushq %rbp' in trampoline prologue
  builder.AlignNextPush(16);

  ArgumentsListBuilder args(builder.Top());
  args.Push((void*)trampoline);
  args.Push(arg);

  // Reserve space for stack-saved context
  builder.Allocate(sizeof(StackSavedMachineContext));

  auto* stack_saved_context = (StackSavedMachineContext*)builder.Top();
  stack_saved_context->rip = (void*)MachineContextTrampoline;

  return stack_saved_context;
}

void MachineContext::Setup(wheels::MutableMemView stack, Trampoline trampoline, void* arg) {
  rsp_ = SetupStack(stack, trampoline, arg);
}

void MachineContext::SwitchTo(MachineContext& target) {
  SwitchMachineContext(&rsp_, &target.rsp_);
}

}  // namespace context
