/*
 * relay.c
 *
 *  Created on: Jul 11, 2026
 *      Author: marco
 */

#include "cmsis_os2.h"
#include "relay.h"

int32_t now;

// indicators
static indicator_state_t left_indicator_state;
static indicator_state_t right_indicator_state;

static bool input_pressed(
    indicator_state_t *state,
    const gpio_t *input
) {
	const GPIO_PinState pin = HAL_GPIO_ReadPin(input->port, input->pin);

    const bool pressed =
    	pin == GPIO_PIN_SET &&
        state->previous_input == GPIO_PIN_RESET;

    state->previous_input = pin;

    return pressed;
}

void indicator_action(indicator_state_t *state, const gpio_t *input, const gpio_t *output) {;
	if (input_pressed(state, input)) {
		// toggle switch
        state->enabled = !state->enabled;

        if (!state->enabled) {
            state->relay_state = false;
            write_gpio(output, GPIO_PIN_RESET);
            return;
        }

        state->relay_state = true;
        write_gpio(output, GPIO_PIN_SET);

        state->next_toggle = now + INDICATOR_INTERVAL;
    }

    if (
        state->enabled &&
        (int32_t)(now - state->next_toggle) >= 0
    ) {
        state->relay_state = !state->relay_state;
        state->next_toggle += INDICATOR_INTERVAL;

        write_gpio(output, state->relay_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}

void left_indicator_action() {
	indicator_action(&left_indicator_state, &LEFT_INDICATOR_IN, &LEFT_INDICATOR_OUT);
}

void right_indicator_action() {
	indicator_action(&right_indicator_state, &RIGHT_INDICATOR_IN, &RIGHT_INDICATOR_OUT);
}

// hazard
static indicator_state_t hazard_state;

static inline void write_hazard(const GPIO_PinState state)
{
    write_gpio(&HAZARD_OUT0, state);
    write_gpio(&HAZARD_OUT1, state);
}

void hazard_action() {
	if (input_pressed(&hazard_state, &HAZARD_IN)) {
		// toggle switch
		hazard_state.enabled = !hazard_state.enabled;

		if (!hazard_state.enabled) {
			hazard_state.relay_state = false;
			write_hazard(GPIO_PIN_RESET);
			return;
		}

		hazard_state.relay_state = true;
		write_hazard(GPIO_PIN_SET);

		hazard_state.next_toggle = now + INDICATOR_INTERVAL;
	}

	if (
		hazard_state.enabled &&
		(int32_t)(now - hazard_state.next_toggle) >= 0
	) {
		hazard_state.relay_state = !hazard_state.relay_state;
		hazard_state.next_toggle += INDICATOR_INTERVAL;

		write_hazard(hazard_state.relay_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
	}
}

void relay_task(void *argument)
{
	for (;;) {
		now = osKernelGetTickCount();

		left_indicator_action();
		right_indicator_action();
		hazard_action();

		osDelay(5);
	}
}
