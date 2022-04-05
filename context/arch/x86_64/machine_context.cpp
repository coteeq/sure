#include <context/arch/x86_64/machine_context.hpp>

#include <wheels/support/panic.hpp>

//////////////////////////////////////////////////////////////////////

extern "C" void* SetupMachineContext(void* stack,
                                     void* trampoline,
                                     void* arg);

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

}  // namespace context
