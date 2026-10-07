#include "static_state.h"

#include "time.h"

static_state::static_state(uint8_t pin_index, uint32_t duration_in_ms)
    : state(), pin_index(pin_index), duration_in_ms(duration_in_ms),
      start_time_in_ms(0), pin{} {}

void static_state::init() { pin = get_digital_pin(pin_index, OUTPUT_MODE); }

void static_state::start() {
  pin.set_high(&pin);
  start_time_in_ms = get_current_time_in_milliseconds();
}

void static_state::turn_off() { pin.set_low(&pin); }

state_result static_state::update() {
  uint32_t current_time_in_ms = get_current_time_in_milliseconds();
  if (current_time_in_ms - start_time_in_ms >= duration_in_ms) {
    turn_off();
    return state_result::COMPLETED;
  }
  return state_result::WAIT;
}
