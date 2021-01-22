#pragma once

#include <context/trampoline.hpp>

#include <wheels/support/memspan.hpp>

namespace context {

struct MachineContext {
  void* rsp_;

  void Setup(wheels::MemSpan stack, Trampoline trampoline);
  void SwitchTo(MachineContext& tagret);
};

}  // namespace context
