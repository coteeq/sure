#pragma once

#include <context/trampoline.hpp>

#include <wheels/support/mem_view.hpp>

namespace context {

struct MachineContext {
  void* rsp_;

  void Setup(wheels::MutableMemView stack, Trampoline trampoline, void* arg);

  void SwitchTo(MachineContext& target);
};

}  // namespace context
