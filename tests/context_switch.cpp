#include <sure/context.hpp>
#include <sure/stack/mmap.hpp>

#include <wheels/test/test_framework.hpp>

#include <iostream>

using namespace sure;

namespace {

ExecutionContext main_ctx;

Stack stack = Stack::AllocateBytes(128 * 1024);
ExecutionContext child_ctx;

}  // namespace anonymous

struct Runner : ITrampoline {

  void Bar() {
  }

  void Foo() {
    Bar();
  }

  // Trampoline
  void Run() noexcept {
    std::cout << "Child" << std::endl;

    Foo();

    child_ctx.SwitchTo(main_ctx);

    std::cout << "Child" << std::endl;

    child_ctx.ExitTo(main_ctx);
  }
};

TEST_SUITE(ExecutionContext1) {
  SIMPLE_TEST(Switch) {
    Runner runner;
    child_ctx.Setup(stack.MutView(), &runner);
    //std::cout << child_ctx.rsp_ << std::endl;

    std::cout << "Parent" << std::endl;
    main_ctx.SwitchTo(child_ctx);
    std::cout << "Parent" << std::endl;
    main_ctx.SwitchTo(child_ctx);
    std::cout << "Parent" << std::endl;
  }
}
