#pragma once

#include <wheels/memory/view.hpp>

#include <sanitizer/tsan_interface.h>

namespace sure {

struct SanitizerContext {
  ~SanitizerContext() {
    if (hold_fiber_) {
      __tsan_destroy_fiber(fiber_);
    }
  }

  void Setup(wheels::MutableMemView) {
    hold_fiber_ = true;
    fiber_ = __tsan_create_fiber(0);
  }

  void AfterStart() {
    // Nop
  }

  void BeforeSwitch(SanitizerContext& target) {
    fiber_ = __tsan_get_current_fiber();
    __tsan_switch_to_fiber(target.fiber_, 0);
  }

  void AfterSwitch() {
    // Nop
  }

  void BeforeExit(SanitizerContext& target) {
    __tsan_switch_to_fiber(target.fiber_, 0);
  }

 private:
  bool hold_fiber_{false};
  void* fiber_;
};

}  // namespace sure
