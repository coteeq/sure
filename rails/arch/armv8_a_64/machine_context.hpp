#pragma once

#include <rails/trampoline.hpp>

#include <wheels/memory/view.hpp>

namespace rails {

// Target architecture: armv8-a

struct MachineContext {
  void* rsp_;

  void Setup(wheels::MutableMemView stack, ITrampoline* trampoline);

  void SwitchTo(MachineContext& target);
};

}  // namespace rails
