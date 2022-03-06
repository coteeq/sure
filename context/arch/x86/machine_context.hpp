#pragma once

#include <context/trampoline.hpp>

#include <wheels/memory/view.hpp>

namespace context {

// Target architecture: x86-64

struct MachineContext {
  void* rsp_;

  void Setup(wheels::MutableMemView stack, ITrampoline* trampoline);

  void SwitchTo(MachineContext& target);
};

}  // namespace context
