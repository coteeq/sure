#include <sure/machine/ucontext/context.hpp>

namespace sure {

struct BrokenDownPtr {
  int lo;
  int hi;
};

union Ptr {
  BrokenDownPtr bits;
  ITrampoline* ptr;
};

static_assert(sizeof(int) == 4);

static void MachineContextTrampoline(int hi, int lo) {
  Ptr ptr{.bits = {lo, hi}};

  ITrampoline* t = ptr.ptr;
  t->Run();
}

typedef void (*UcontextTrampoline)(void);

MachineContext::MachineContext() {
  //getcontext(&context);
}

void MachineContext::Setup(wheels::MutableMemView stack, ITrampoline* trampoline) {
  getcontext(&context);

  context.uc_link = 0;
  context.uc_stack.ss_sp = stack.Data();
  context.uc_stack.ss_size = stack.Size();
  context.uc_stack.ss_flags = 0;

  Ptr ptr {.ptr = trampoline};

  makecontext(&context, (UcontextTrampoline)MachineContextTrampoline, 2, ptr.bits.hi, ptr.bits.lo);
}

void MachineContext::SwitchTo(MachineContext& target) {
  swapcontext(&context, &target.context);
}

}  // namespace sure
