#include <context/context.hpp>
#include <context/stack.hpp>

#include <wheels/test/test_framework.hpp>

#include <iostream>

using namespace context;

ExecutionContext main_ctx;

Stack stack = Stack::AllocatePages(8);
ExecutionContext child_ctx;

void ChildRoutine() {
  child_ctx.AfterStart();

  std::cout << "Child" << std::endl;

  child_ctx.SwitchTo(main_ctx);

  std::cout << "Child" << std::endl;

  child_ctx.SwitchTo(main_ctx);
}

TEST_SUITE(ExecutionContext1) {
  SIMPLE_TEST(SwitchContext) {
    child_ctx.Setup(stack.View(), ChildRoutine);
    //std::cout << child_ctx.rsp_ << std::endl;

    std::cout << "Parent" << std::endl;
    main_ctx.SwitchTo(child_ctx);
    std::cout << "Parent" << std::endl;
    main_ctx.SwitchTo(child_ctx);
    std::cout << "Parent" << std::endl;
  }
}
