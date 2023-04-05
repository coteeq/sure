#include <wheels/test/framework.hpp>

#include <sure/context.hpp>
#include <sure/stack.hpp>

#include <wheels/memory/view_of.hpp>

#include <iostream>

sure::ExecutionContext* jumper_ctx;

struct Jumper : sure::ITrampoline {
  Jumper(sure::ExecutionContext* main)
      : main_ctx(main) {
  }

  void Run() noexcept final {
    std::cout << "Jumper started" << std::endl;

    sure::ExecutionContext my_ctx;

    jumper_ctx = &my_ctx;
    my_ctx.SwitchTo(*main_ctx);

    std::cout << "Jumper resumed" << std::endl;

    my_ctx.ExitTo(*main_ctx);
  }

  sure::ExecutionContext* main_ctx;
  char stack[64 * 1024];
};

void StatelessTest() {
  sure::ExecutionContext main_ctx;

  Jumper jumper{&main_ctx};

  {
    sure::ExecutionContext init_ctx;
    init_ctx.Setup(wheels::MutViewOf(jumper.stack), &jumper);

    std::cout << "Switch to jumper" << std::endl;

    main_ctx.SwitchTo(init_ctx);

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
