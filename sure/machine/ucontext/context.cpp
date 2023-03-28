#include <sure/machine/ucontext/context.hpp>

namespace sure {

/*
 * Workaround for legacy `ucontext` API:
 *
 * makecontext and ucontext trampoline expect arguments of type int even on 64-bit arch,
 * so we have to break down pointer to ITrampoline to a couple of 32-bit integers
 * (lo and hi part of the address) and pass them to trampoline separately to avoid UB
 *
 * Actually, any sane implementation of `ucontext` for 64-bit arch will support void*
 * as trampoline argument, but formally it still will be UB
 */

namespace {

static_assert(sizeof(int) == 4);

struct PtrRepr {
  int lo;
  int hi;
};

union Ptr {
  PtrRepr repr;
  void* addr;
};

}  // namespace

static void MachineContextTrampoline(int lo, int hi) {
  Ptr ptr{.repr = {lo, hi}};

  ITrampoline* t = (ITrampoline*)ptr.addr;
  t->Run();
}

MachineContext::MachineContext() {
  //getcontext(&context);
}

typedef void (*UcontextTrampolineType)(void);

void MachineContext::Setup(wheels::MutableMemView stack, ITrampoline* trampoline) {
  getcontext(&context);

  context.uc_link = 0;
  context.uc_stack.ss_sp = stack.Data();
  context.uc_stack.ss_size = stack.Size();
  context.uc_stack.ss_flags = 0;

  Ptr ptr{.addr = trampoline};

  makecontext(&context, (UcontextTrampolineType)MachineContextTrampoline, 2, ptr.repr.lo, ptr.repr.hi);
}

void MachineContext::SwitchTo(MachineContext& target) {
  swapcontext(&context, &target.context);
}

}  // namespace sure
