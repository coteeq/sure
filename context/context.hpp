#pragma once

#include <context/trampoline.hpp>
#include <context/stack_view.hpp>

#include <cstdlib>
#include <cstdint>

namespace context {

struct ExecutionContext {
  // Callee-saved registers are pushed into the suspended stack
  void* rsp_;

  // Prepare execution context for running trampoline function
  void Setup(StackView stack, Trampoline trampoline);

  // Symmetric Control Transfer
  // 1) Save the current execution context to 'this'
  // 2) Activate 'target' context.
  void SwitchTo(ExecutionContext& target);
};

}  // namespace context
