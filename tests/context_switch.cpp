#include <rails/context.hpp>
#include <rails/stack/mmap.hpp>

#include <wheels/test/test_framework.hpp>

#include <iostream>

using namespace rails;

ExecutionContext main_ctx;

Stack stack = Stack::AllocatePages(8);
ExecutionContext child_ctx;

struct Runner : ITrampoline {

  void Bar() {
  }

  void Foo() {
    Bar();
  }

  // Trampoline
  void Run() {
    std::cout << "Child" << std::endl;

    Foo();

    child_ctx.SwitchTo(main_ctx);

    std::cout << "Child" << std::endl;

    child_ctx.ExitTo(main_ctx);
  }
};

TEST_SUITE(ExecutionContext1) {
  SIMPLE_TEST(SwitchContext) {
    Runner runner;
    child_ctx.Setup(stack.View(), &runner);
    //std::cout << child_ctx.rsp_ << std::endl;

    std::cout << "Parent" << std::endl;
    main_ctx.SwitchTo(child_ctx);
    std::cout << "Parent" << std::endl;
    main_ctx.SwitchTo(child_ctx);
    std::cout << "Parent" << std::endl;
  }
}
