#pragma once

namespace context {

struct ITrampoline {
  virtual ~ITrampoline() = default;

  // Never returns
  virtual void Run() = 0;
};

}  // namespace context
