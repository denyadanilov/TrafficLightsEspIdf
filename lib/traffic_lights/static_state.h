#ifndef LIB_TRAFFIC_LIGHTS_STATIC_STATE_H
#define LIB_TRAFFIC_LIGHTS_STATIC_STATE_H

#include <stdint.h>

#include "pin.h"
#include "state.h"

class static_state : public state {
private:
  const uint8_t pin_index;
  const uint32_t duration_in_ms;
  uint32_t start_time_in_ms;
  digital_pin_t pin;

public:
  static_state(uint8_t pin_index, uint32_t duration_in_ms);

  void init() override;
  void start() override;
  void turn_off() override;
  state_result update() override;
};

#endif // LIB_TRAFFIC_LIGHTS_STATIC_STATE_H
