#include "app.h"
#include "configuration.h"
#include "green_blinking_state.h"
#include "green_state.h"
#include "red_state.h"
#include "red_yellow_state.h"
#include "task.h"
#include "toggle_switch.h"
#include "yellow_blinking_state.h"
#include "yellow_state.h"

namespace {

constexpr uint32_t TASK_STACK_DEPTH = 2048;
constexpr uint8_t TASK_PRIORITY = 5;

green_state green;
green_blinking_state green_blinking;
yellow_state yellow;
red_state red;
red_yellow_state red_yellow;
yellow_blinking_state yellow_blinking;

state *const states[] = {&green, &green_blinking, &yellow, &red, &red_yellow};
constexpr size_t STATES_COUNT = sizeof(states) / sizeof(states[0]);

toggle_switch mode_switch(BUTTON_PIN);

size_t current_state_index = 0;
switch_state current_mode = switch_state::OFF;

void switch_mode(switch_state mode) {
  states[current_state_index]->turn_off();
  yellow_blinking.turn_off();
  current_mode = mode;
  if (mode == switch_state::ON) {
    yellow_blinking.start();
  } else {
    current_state_index = 0;
    states[current_state_index]->start();
  }
}

void run_sequence() {
  if (states[current_state_index]->update() == state_result::COMPLETED) {
    current_state_index = (current_state_index + 1) % STATES_COUNT;
    states[current_state_index]->start();
  }
}

void update_task(void *) {
  switch_state requested_mode = mode_switch.update();
  if (requested_mode != current_mode) {
    switch_mode(requested_mode);
  }

  if (current_mode == switch_state::ON) {
    yellow_blinking.update();
  } else {
    run_sequence();
  }
}

task_context_t update_task_context = {update_task, nullptr};

} // namespace

void app_init() {
  for (state *s : states) {
    s->init();
  }
  yellow_blinking.init();
  mode_switch.init();
}

void app_run() {
  current_mode = mode_switch.update();
  switch_mode(current_mode);
  schedule_task(&update_task_context, "traffic_lights_update", TASK_STACK_DEPTH,
                TASK_PRIORITY);
}
