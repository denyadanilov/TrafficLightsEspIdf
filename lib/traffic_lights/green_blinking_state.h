#ifndef LIB_TRAFFIC_LIGHTS_GREEN_BLINKING_STATE_H
#define LIB_TRAFFIC_LIGHTS_GREEN_BLINKING_STATE_H

#include "configuration.h"
#include "adapter/pin.hpp"
#include "state.h"
#include "adapter/time.hpp"

class green_blinking_state : public state {
private:
  const uint32_t blink_cycle_duration_in_ms = GREEN_BLINK_DURATION_MS * 2;
  const uint32_t total_blink_duration_in_ms =
      blink_cycle_duration_in_ms * GREEN_BLINK_NUMBER;

  eaf::pin green_pin;
  uint32_t start_time_in_ms;

public:
  green_blinking_state()
      : green_pin(GREEN_PIN, eaf::pin_mode::OUTPUT_MODE), start_time_in_ms(0) {}

  void init() override {}

  void start() override {
    green_pin.set_high();
    start_time_in_ms = eaf::get_current_time_in_milliseconds();
  }

  void turn_off() override { green_pin.set_low(); }

  state_result update() override {
    uint32_t current_time_in_ms =
        eaf::get_current_time_in_milliseconds();
    uint32_t elapsed_time_in_ms = current_time_in_ms - start_time_in_ms;

    if (elapsed_time_in_ms >= total_blink_duration_in_ms) {
      turn_off();
      return state_result::COMPLETED;
    }

    if ((elapsed_time_in_ms % blink_cycle_duration_in_ms) <
        GREEN_BLINK_DURATION_MS) {
      green_pin.set_high();
    } else {
      green_pin.set_low();
    }

    return state_result::WAIT;
  }
};

#endif // LIB_TRAFFIC_LIGHTS_GREEN_BLINKING_STATE_H
