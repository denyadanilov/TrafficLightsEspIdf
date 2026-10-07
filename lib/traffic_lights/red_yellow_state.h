#ifndef LIB_TRAFFIC_LIGHTS_RED_YELLOW_STATE_H
#define LIB_TRAFFIC_LIGHTS_RED_YELLOW_STATE_H

#include "configuration.h"
#include "static_state.h"

// Decorates a red static_state with a yellow one so both lights run together.
class red_yellow_state : public state {
private:
  static_state red;
  static_state yellow;

public:
  red_yellow_state()
      : red(RED_PIN, YELLOW_LIGHT_DURATION_MS),
        yellow(YELLOW_PIN, YELLOW_LIGHT_DURATION_MS) {}

  void init() override {
    red.init();
    yellow.init();
  }

  void start() override {
    red.start();
    yellow.start();
  }

  void turn_off() override {
    red.turn_off();
    yellow.turn_off();
  }

  state_result update() override {
    state_result red_result = red.update();
    state_result yellow_result = yellow.update();
    if (red_result == state_result::COMPLETED &&
        yellow_result == state_result::COMPLETED) {
      return state_result::COMPLETED;
    }
    return state_result::WAIT;
  }
};

#endif // LIB_TRAFFIC_LIGHTS_RED_YELLOW_STATE_H
