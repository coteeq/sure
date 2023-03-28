#pragma once

#include <wheels/memory/view.hpp>

namespace sure {

struct SanitizerContext {
  void Setup(wheels::MutableMemView) {
    // Nop
  }

  void AfterStart() {
    // Nop
  }

  void BeforeSwitch(SanitizerContext& /*target*/) {
    // Nop
  }

  void AfterSwitch() {
    // Nop
  }

  void BeforeExit(SanitizerContext& /*target*/) {
    // Nop
  }
};

}  // namespace sure
