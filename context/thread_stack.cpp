#include <context/thread_stack.hpp>

#include <wheels/support/assert.hpp>

#include <pthread.h>

#define CHECK_RESULT(retcode, error) \
  WHEELS_VERIFY(retcode == 0, error << ": " << errno)

namespace context {

using wheels::MemSpan;

//////////////////////////////////////////////////////////////////////

static MemSpan GetThisThreadStack() {
  void* start{nullptr};
  size_t size{0};

#if APPLE
  auto self = pthread_self();
  start = pthread_get_stackaddr_np(self);
  size = pthread_get_stacksize_np(self);

#elif LINUX
  pthread_attr_t attrs;
  int retcode;

  retcode = pthread_getattr_np(pthread_self(), &attrs);
  CHECK_RESULT(retcode, "cannot get thread attributes");

  retcode = pthread_attr_getstack(&attrs, &start, &size);
  CHECK_RESULT(retcode, "cannot get stack address and size");

  retcode = pthread_attr_destroy(&attrs);
  CHECK_RESULT(retcode, "pthread_attr_destroy failed");
#endif

  return {static_cast<char*>(start), size};
}

//////////////////////////////////////////////////////////////////////

struct ThreadStackBounds {
  ThreadStackBounds() {
    Initialize();
  }

  void Initialize() {
    stack_ = GetThisThreadStack();
  }

  MemSpan Get() const {
    return stack_;
  }

  MemSpan stack_;
};

MemSpan ThisThreadStack() {
  static thread_local ThreadStackBounds bounds;
  return bounds.Get();
}

}  // namespace context
