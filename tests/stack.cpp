#include <rails/stack/mmap.hpp>

#include <wheels/test/test_framework.hpp>

using rails::Stack;

TEST_SUITE(Stacks) {
  SIMPLE_TEST(AllocateBytes1) {
    size_t page_size = wheels::MmapAllocation::PageSize();

    auto stack = Stack::AllocateBytes(/*at_least*/1);
    ASSERT_EQ(stack.AllocationSize(), page_size * 2);
  }

  SIMPLE_TEST(AllocateBytes2) {
    size_t page_size = wheels::MmapAllocation::PageSize();

    auto stack = Stack::AllocateBytes(/*at_least*/page_size * 3 + 1);
    ASSERT_EQ(stack.AllocationSize(), page_size * 5);
  }
}
