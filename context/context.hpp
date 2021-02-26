#pragma once

#include <wheels/support/memspan.hpp>

#include <cstdlib>
#include <cstdint>

namespace context {

using wheels::MemSpan;

typedef void (*Trampoline)();

struct ExecutionContext {
  // Callee-saved registers are pushed into the suspended stack
  void* rsp_;

  // Prepare execution context for running trampoline function
  void Setup(MemSpan stack, Trampoline trampoline);

  // Symmetric Control Transfer
  // 1) Save the current execution context to 'this'
  // 2) Activate 'target' context.
  void SwitchTo(ExecutionContext& target);
};

}  // namespace context
