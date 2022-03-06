#pragma once

#include <context/trampoline.hpp>

#include <wheels/memory/view.hpp>

namespace context {

struct MachineContext {
  void* rsp_;

  void Setup(wheels::MutableMemView stack, ITrampoline* trampoline);

  void SwitchTo(MachineContext& target);
};

}  // namespace context
