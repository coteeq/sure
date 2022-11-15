#include <sure/stack/thread.hpp>

#include <wheels/test/test_framework.hpp>


TEST_SUITE(ThreadStack) {
  SIMPLE_TEST(MainThread) {
    auto stack = sure::ThisThreadStack();
    printf("This thread stack: {%p, %zu}\n", (void*)stack.Data(), stack.Size());

    ASSERT_NE(stack.Data(), nullptr);
    ASSERT_EQ(stack.Data(), sure::ThisThreadStack().Data());
  }
}
