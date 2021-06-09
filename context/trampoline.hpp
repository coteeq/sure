#pragma once

namespace context {

typedef void (*Trampoline)();

typedef void (*TrampolineWithArgument)(void*);

}  // namespace context
