#include <context/machine_context.hpp>

#include <context/stack_builder.hpp>

#include <wheels/support/panic.hpp>

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
static void MachineContextTrampoline(void*, void*, void*, void*, void*, void*, void* arg7) {
  ITrampoline* t = (ITrampoline*)arg7;
  t->Run();
  WHEELS_PANIC("ITrampoline::Run should never return control");
}

static void* SetupStack(wheels::MutableMemView stack, ITrampoline* trampoline) {
  // https://eli.thegreenplace.net/2011/02/04/where-the-top-of-the-stack-is-on-x86/

  StackBuilder builder(stack.Back());

  // Preallocate space for arguments
  builder.Allocate(sizeof(uintptr_t) * 3);

  // Ensure trampoline will get 16-byte aligned frame pointer (rbp)
  // 'Next' here means first 'pushq %rbp' in trampoline prologue
  builder.AlignNextPush(16);

  ArgumentsListBuilder args(/*rbp=*/builder.Top());
  args.Add((void*)trampoline);

  // Reserve space for stack-saved context
  builder.Allocate(sizeof(StackSavedMachineContext));

  auto* stack_saved_context = (StackSavedMachineContext*)builder.Top();
  stack_saved_context->rip = (void*)MachineContextTrampoline;

  return stack_saved_context;
}

void MachineContext::Setup(wheels::MutableMemView stack, ITrampoline* trampoline) {
  rsp_ = SetupStack(stack, trampoline);
}

void MachineContext::SwitchTo(MachineContext& target) {
  SwitchMachineContext(&rsp_, &target.rsp_);
}

}  // namespace context
