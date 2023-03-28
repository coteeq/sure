#pragma once

#include <sure/trampoline.hpp>

#include <wheels/memory/view.hpp>

namespace sure {

// Target architecture: armv8-a

struct MachineContext {
  void* rsp_;

  void Setup(wheels::MutableMemView stack, ITrampoline* trampoline);

  void SwitchTo(MachineContext& target);
};

}  // namespace sure
