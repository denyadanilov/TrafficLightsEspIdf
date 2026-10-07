#include "toggle_switch.h"

#include "interrupt.h"
#include "pin.h"
#include "utils.h"

toggle_switch *toggle_switch::instance = nullptr;

toggle_switch::toggle_switch(uint8_t pin_index)
    : pin_index(pin_index), current_state(switch_state::OFF) {}

void toggle_switch::init() {
  instance = this;
  get_digital_pin(pin_index, INPUT_PULLUP_MODE);
  attach_interrupt(pin_index, isr, FALLING_STATE);
}

switch_state toggle_switch::update() { return current_state; }

IRAM_LOCATED void toggle_switch::isr() {
  if (instance == nullptr) {
    return;
  }
  instance->current_state = instance->current_state == switch_state::OFF
                                ? switch_state::ON
                                : switch_state::OFF;
}
