#include <wheels/test/framework.hpp>

#include <sure/context.hpp>
#include <sure/stack.hpp>

#include <wheels/memory/view_of.hpp>

#include <iostream>

namespace {

sure::ExecutionContext main_ctx;

sure::ExecutionContext* jumper_ctx;

}  // namespace anonymous

struct Jumper : sure::ITrampoline {
  void Run() noexcept final {
    std::cout << "Jumper started" << std::endl;

    sure::ExecutionContext my_ctx;

    jumper_ctx = &my_ctx;
    my_ctx.SwitchTo(main_ctx);

    std::cout << "Jumper resumed" << std::endl;

    my_ctx.ExitTo(main_ctx);
  }

  char stack[64 * 1024];
};

void StatelessTest() {
  Jumper jumper{};

  {
    sure::ExecutionContext ctx_init;
    ctx_init.Setup(wheels::MutViewOf(jumper.stack), &jumper);

    std::cout << "Switch to jumper" << std::endl;

    main_ctx.SwitchTo(ctx_init);

    std::cout << "Main resumed" << std::endl;
  }

  std::cout << "Resume jumper" << std::endl;

  main_ctx.SwitchTo(*jumper_ctx);

  std::cout << "Completed" << std::endl;
}

// NB: Run with sanitizers!

TEST_SUITE(ExecutionContext2) {
  SIMPLE_TEST(Stateless) {
    StatelessTest();
  }
}
