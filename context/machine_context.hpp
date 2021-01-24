#pragma once

#include <context/trampoline.hpp>

#include <wheels/support/memspan.hpp>

namespace context {

using StackView = wheels::MemSpan;

struct MachineContext {
  void* rsp_;

  void Setup(StackView stack, Trampoline trampoline);
  void SwitchTo(MachineContext& target);
};

}  // namespace context
