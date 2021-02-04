#pragma once

#include <context/stack_view.hpp>
#include <context/trampoline.hpp>

namespace context {

struct MachineContext {
  void* rsp_;

  void Setup(StackView stack, Trampoline trampoline);
  void SwitchTo(MachineContext& target);
};

}  // namespace context
