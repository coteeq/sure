#include <context/stack.hpp>

#include <utility>

using wheels::MmapAllocation;

namespace context {

Stack::Stack(MmapAllocation allocation) : allocation_(std::move(allocation)) {
}

Stack Stack::AllocatePages(size_t count) {
  auto allocation = MmapAllocation::AllocatePages(count + 1);
  // Guard page
  allocation.ProtectPages(/*offset=*/0, /*count=*/1);
  return Stack{std::move(allocation)};
}

char* Stack::Bottom() const {
  return (char*)((std::uintptr_t*)allocation_.End() - 1);
}

}  // namespace context
