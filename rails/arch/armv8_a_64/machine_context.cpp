#include <rails/arch/armv8_a_64/machine_context.hpp>

#include <wheels/support/panic.hpp>

//////////////////////////////////////////////////////////////////////

extern "C" void* SetupMachineContext(void* stack,
                                     void* trampoline,
                                     void* arg);

// Switch between MachineContext-s
extern "C" void SwitchMachineContext(void* from_rsp, void* to_rsp);

//////////////////////////////////////////////////////////////////////

namespace rails {

void MachineContextTrampoline(void*, void*, void*, void*, void*, void*, void*, void*, void* arg9) {
  ITrampoline* t = (ITrampoline*)arg9;
  t->Run();
}

//////////////////////////////////////////////////////////////////////

void MachineContext::Setup(wheels::MutableMemView stack, ITrampoline* trampoline) {
  rsp_ = SetupMachineContext((void*)stack.End(), (void*)MachineContextTrampoline, trampoline);
}

void MachineContext::SwitchTo(MachineContext& target) {
  SwitchMachineContext(&rsp_, &target.rsp_);
}

}  // namespace rails
