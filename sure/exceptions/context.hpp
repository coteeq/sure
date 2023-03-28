#pragma once

#if defined(UNIX) and defined(SURE_CAPTURE_EXCEPTIONS_CONTEXT)

#include <sure/exceptions/unix/context.hpp>

#else

#include <sure/exceptions/nop/context.hpp>

#endif
