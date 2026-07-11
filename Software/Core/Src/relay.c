/*
 * relay.c
 *
 *  Created on: Jul 11, 2026
 *      Author: marco
 */

#include "relay.h"

#define RELAY_POLL_MS      5
#define RELAY_DEBOUNCE_MS  20
#define RELAY_DEBOUNCE_COUNT \
    (RELAY_DEBOUNCE_MS / RELAY_POLL_MS)

void relay_task(void *argument)
{
    GPIO_PinState stable_states[ARRAY_SIZE(relay_definitions)];
    GPIO_PinState sampled_states[ARRAY_SIZE(relay_definitions)];
    uint8_t stable_counts[ARRAY_SIZE(relay_definitions)] = {0};

    for (size_t i = 0; i < ARRAY_SIZE(relay_definitions); ++i) {
        const relay_definition_t *def = &relay_definitions[i];

        stable_states[i] =
            HAL_GPIO_ReadPin(def->input_port, def->input_pin);

        sampled_states[i] = stable_states[i];

        HAL_GPIO_WritePin(
            def->output_port,
            def->output_pin,
            stable_states[i]
        );
    }

    for (;;) {
        for (size_t i = 0; i < ARRAY_SIZE(relay_definitions); ++i) {
            const relay_definition_t *def = &relay_definitions[i];

            const GPIO_PinState sample =
                HAL_GPIO_ReadPin(def->input_port, def->input_pin);

            if (sample != sampled_states[i]) {
                sampled_states[i] = sample;
                stable_counts[i] = 0;
                continue;
            }

            if (sample == stable_states[i]) {
                stable_counts[i] = 0;
                continue;
            }

            if (++stable_counts[i] >= RELAY_DEBOUNCE_COUNT) {
                stable_states[i] = sample;
                stable_counts[i] = 0;

                HAL_GPIO_WritePin(
                    def->output_port,
                    def->output_pin,
                    sample
                );
            }
        }

        osDelay(RELAY_POLL_MS);
    }
}
