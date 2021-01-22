#include <context/exceptions.hpp>

#include <stdexcept>
// memcpy
#include <cstring>

namespace __cxxabiv1 {  // NOLINT

struct __cxa_eh_globals;  // NOLINT

// NOLINTNEXTLINE
extern "C" __cxa_eh_globals* __cxa_get_globals() noexcept;

}  // namespace __cxxabiv1

namespace context {

void SwitchExceptionsContext(ExceptionsContext& from, ExceptionsContext& to) {
  static constexpr size_t kStateSize = sizeof(ExceptionsContext);

  auto* this_thread_exceptions = __cxxabiv1::__cxa_get_globals();
  memcpy(from.exceptions_state_buf_, this_thread_exceptions, kStateSize);
  memcpy(this_thread_exceptions, to.exceptions_state_buf_, kStateSize);
}

}  // namespace context
