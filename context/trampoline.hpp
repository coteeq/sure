#pragma once

namespace context {

struct ITrampoline {
  virtual ~ITrampoline() = default;

  virtual void Run() = 0;
};

}  // namespace context
