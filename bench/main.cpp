#include <iostream>

#include <context/context.hpp>
#include <context/stack/mmap.hpp>

#include <iostream>

using namespace context;

struct Runner : ITrampoline {
 public:
  Runner(size_t steps)
      : steps_left_(steps),
        stack_(Stack::AllocateBytes(64 * 1024)) {
    self_.Setup(stack_.View(), this);
  }

  size_t StepsLeft() const {
    return steps_left_;
  }

  void MakeStep() {
    parent_.SwitchTo(self_);
  }

  // Trampoline
  void Run() {
    while (steps_left_ > 1) {
      --steps_left_;
      self_.SwitchTo(parent_);
    }

    --steps_left_;
    self_.ExitTo(parent_);
  }

 private:
  size_t steps_left_;
  Stack stack_;
  ExecutionContext self_;
  ExecutionContext parent_;
};

int main() {

  Runner runner{20'000'000};
  while (runner.StepsLeft() > 0) {
    runner.MakeStep();
  }

  return 0;
}
