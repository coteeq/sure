#include <sure/stack/mmap.hpp>

#include <utility>

using wheels::MmapAllocation;

namespace sure {

Stack::Stack(MmapAllocation allocation) : allocation_(std::move(allocation)) {
}

Stack Stack::AllocatePages(size_t count) {
  auto allocation = MmapAllocation::AllocatePages(count + 1);
  // Guard page
  allocation.ProtectPages(/*offset=*/0, /*count=*/1);
  return Stack{std::move(allocation)};
}

Stack Stack::AllocateBytes(size_t at_least) {
  const size_t page_size = MmapAllocation::PageSize();

  size_t pages = at_least / page_size;
  if (at_least % page_size != 0) {
    ++pages;
  }

  return Stack::AllocatePages(/*count=*/pages);
}

wheels::MutableMemView Stack::MutView() {
  return allocation_.MutView();
}

Stack Stack::Acquire(wheels::MutableMemView view) {
  return Stack(MmapAllocation::Acquire(view));
}

wheels::MutableMemView Stack::Release() {
  return allocation_.Release();
}

}  // namespace sure
