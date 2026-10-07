#ifndef LIB_TRAFFIC_LIGHTS_YELLOW_BLINKING_STATE_H
#define LIB_TRAFFIC_LIGHTS_YELLOW_BLINKING_STATE_H

#include "configuration.h"
#include "pin.h"
#include "state.h"
#include "time.h"

// Blinks forever: update() never returns COMPLETED.
class yellow_blinking_state : public state {
private:
  const uint32_t blink_cycle_duration_in_ms = YELLOW_BLINK_DURATION_MS * 2;

  digital_pin_t yellow_pin;
  uint32_t start_time_in_ms;

public:
  void init() override {
    yellow_pin = get_digital_pin(YELLOW_PIN, OUTPUT_MODE);
  }

  void start() override {
    yellow_pin.set_high(&yellow_pin);
    start_time_in_ms = get_current_time_in_milliseconds();
  }

  void turn_off() override { yellow_pin.set_low(&yellow_pin); }

  state_result update() override {
    uint32_t elapsed_time_in_ms =
        get_current_time_in_milliseconds() - start_time_in_ms;

    if ((elapsed_time_in_ms % blink_cycle_duration_in_ms) <
        YELLOW_BLINK_DURATION_MS) {
      yellow_pin.set_high(&yellow_pin);
    } else {
      yellow_pin.set_low(&yellow_pin);
    }

    return state_result::WAIT;
  }
};

#endif // LIB_TRAFFIC_LIGHTS_YELLOW_BLINKING_STATE_H
