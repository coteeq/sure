#include <rails/stack/thread.hpp>

#include <wheels/test/test_framework.hpp>


TEST_SUITE(ThreadStack) {
  SIMPLE_TEST(MainThread) {
    auto stack = rails::ThisThreadStack();
    printf("This thread stack: {%p, %zu}\n", (void*)stack.Data(), stack.Size());

    ASSERT_NE(stack.Data(), nullptr);
    ASSERT_EQ(stack.Data(), rails::ThisThreadStack().Data());
  }
}
