#pragma once

#include <wheels/memory/view.hpp>

#include <sanitizer/tsan_interface.h>

namespace sure {

struct SanitizerContext {
  void Setup(wheels::MutableMemView /*stack*/) {
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
    if (exit_from_ != nullptr) {
      __tsan_destroy_fiber(exit_from_->fiber_);
    }
  }

  void BeforeExit(SanitizerContext& target) {
    target.exit_from_ = this;
    __tsan_switch_to_fiber(target.fiber_, 0);
  }

 private:
  void* fiber_;
  SanitizerContext* exit_from_{nullptr};
};

}  // namespace sure
