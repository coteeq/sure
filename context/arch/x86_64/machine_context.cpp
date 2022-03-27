#include <context/arch/x86_64/machine_context.hpp>

#include <wheels/support/panic.hpp>

//////////////////////////////////////////////////////////////////////

extern "C" void* SetupMachineContext(void* stack,
                                     void* trampoline,
                                     void* arg);

// Switch between MachineContext-s
extern "C" void SwitchMachineContext(void* from_rsp, void* to_rsp);

//////////////////////////////////////////////////////////////////////

namespace context {

// https://eli.thegreenplace.net/2011/09/06/stack-frame-layout-on-x86-64/
void MachineContextTrampoline(void*, void*, void*, void*, void*, void*, void* arg7) {
  ITrampoline* t = (ITrampoline*)arg7;
  t->Run();
}

//////////////////////////////////////////////////////////////////////

void MachineContext::Setup(wheels::MutableMemView stack, ITrampoline* trampoline) {
  rsp_ = SetupMachineContext((void*)stack.End(), (void*)MachineContextTrampoline, trampoline);
}

void MachineContext::SwitchTo(MachineContext& target) {
  SwitchMachineContext(&rsp_, &target.rsp_);
}

}  // namespace context