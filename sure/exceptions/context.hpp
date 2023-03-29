#pragma once

#if defined(UNIX) and defined(SURE_EXCEPTIONS_CONTEXT_unix)

#include <sure/exceptions/unix/context.hpp>

#else

#include <sure/exceptions/nop/context.hpp>

#endif
