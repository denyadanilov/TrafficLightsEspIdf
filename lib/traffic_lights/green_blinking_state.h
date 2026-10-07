#ifndef LIB_TRAFFIC_LIGHTS_GREEN_BLINKING_STATE_H
#define LIB_TRAFFIC_LIGHTS_GREEN_BLINKING_STATE_H

#include "configuration.h"
#include "pin.h"
#include "state.h"
#include "time.h"

class green_blinking_state : public state {
private:
  const uint32_t blink_cycle_duration_in_ms = GREEN_BLINK_DURATION_MS * 2;
  const uint32_t total_blink_duration_in_ms =
      blink_cycle_duration_in_ms * GREEN_BLINK_NUMBER;

  digital_pin_t green_pin;
  uint32_t start_time_in_ms;

public:
  void init() override { green_pin = get_digital_pin(GREEN_PIN, OUTPUT_MODE); }

  void start() override {
    green_pin.set_high(&green_pin);
    start_time_in_ms = get_current_time_in_milliseconds();
  }

  void turn_off() override { green_pin.set_low(&green_pin); }

  state_result update() override {
    uint32_t current_time_in_ms = get_current_time_in_milliseconds();
    uint32_t elapsed_time_in_ms = current_time_in_ms - start_time_in_ms;

    if (elapsed_time_in_ms >= total_blink_duration_in_ms) {
      turn_off();
      return state_result::COMPLETED;
    }

    if ((elapsed_time_in_ms % blink_cycle_duration_in_ms) <
        GREEN_BLINK_DURATION_MS) {
      green_pin.set_high(&green_pin);
    } else {
      green_pin.set_low(&green_pin);
    }

    return state_result::WAIT;
  }
};

#endif // LIB_TRAFFIC_LIGHTS_GREEN_BLINKING_STATE_H
