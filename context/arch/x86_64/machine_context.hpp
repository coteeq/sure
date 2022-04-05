#pragma once

#include <context/trampoline.hpp>

#include <wheels/memory/view.hpp>

// Switch between MachineContext-s
extern "C" void SwitchMachineContext(void* from_rsp, void* to_rsp);

namespace context {

// Target architecture: x86-64

struct MachineContext {
  void* rsp_;

  void Setup(wheels::MutableMemView stack, ITrampoline* trampoline);

  void SwitchTo(MachineContext& target) {
    SwitchMachineContext(&rsp_, &target.rsp_);
  }
};

}  // namespace context
