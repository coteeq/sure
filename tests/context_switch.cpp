#include <sure/context.hpp>
#include <sure/stack/mmap.hpp>

#include <wheels/memory/view_of.hpp>

#include <wheels/test/test_framework.hpp>

#include <iostream>

using namespace sure;

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

    ctx.SwitchTo(*main_ctx);

    std::cout << "Child" << std::endl;

    ctx.ExitTo(*main_ctx);
  }

  char stack[128 * 1024];
  sure::ExecutionContext ctx;
  sure::ExecutionContext* main_ctx;
};

TEST_SUITE(ExecutionContext1) {
  SIMPLE_TEST(Switch) {
    ExecutionContext main_ctx;

    Runner runner;
    runner.main_ctx = &main_ctx;
    runner.ctx.Setup(wheels::MutViewOf(runner.stack), &runner);
    //std::cout << child_ctx.rsp_ << std::endl;

    std::cout << "Parent" << std::endl;
    main_ctx.SwitchTo(runner.ctx);
    std::cout << "Parent" << std::endl;
    main_ctx.SwitchTo(runner.ctx);
    std::cout << "Parent" << std::endl;
  }
}
