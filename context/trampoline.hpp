#pragma once

namespace context {

typedef void (*Trampoline)(void* arg);

typedef void (*TrampolineWithoutArgs)();

}  // namespace context
