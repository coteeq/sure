#pragma once

namespace rails {

// All runnable entities (coroutines, fibers, generators)
// should implement ITrampoline

struct ITrampoline {
  virtual ~ITrampoline() = default;

  // Never returns
  virtual void Run() = 0;
};

}  // namespace rails
