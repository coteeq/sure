#pragma once

namespace context {

// All runnable entities (coroutines, fibers, generators)
// should implement ITrampoline

struct ITrampoline {
  virtual ~ITrampoline() = default;

  // Never returns
  virtual void Run() = 0;
};

}  // namespace context
