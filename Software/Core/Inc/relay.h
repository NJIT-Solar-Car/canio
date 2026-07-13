/*
 * relay.h
 *
 *  Created on: Jul 11, 2026
 *      Author: marco
 */

#ifndef INC_RELAY_H_
#define INC_RELAY_H_

#include "main.h"
#include "stdbool.h"
#include "FreeRTOS.h"

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
} gpio_t;

static void write_gpio(const gpio_t *gpio, const GPIO_PinState state) {
    HAL_GPIO_WritePin(gpio->port, gpio->pin, state);
}

static const gpio_t LEFT_INDICATOR_IN = { GPIO0_GPIO_Port, GPIO0_Pin };
static const gpio_t RIGHT_INDICATOR_IN = { GPIO1_GPIO_Port, GPIO1_Pin };
static const gpio_t HAZARD_IN = { GPIO2_GPIO_Port, GPIO2_Pin};

static const gpio_t LEFT_INDICATOR_OUT = { HighChan0_GPIO_Port, HighChan0_Pin };
static const gpio_t RIGHT_INDICATOR_OUT = { HighChan1_GPIO_Port, HighChan1_Pin };
static const gpio_t HAZARD_OUT0 = { HighChan2_GPIO_Port, HighChan2_Pin };
static const gpio_t HAZARD_OUT1 = { HighChan3_GPIO_Port, HighChan3_Pin};

// indicators
static const TickType_t INDICATOR_INTERVAL = pdMS_TO_TICKS(1000);

typedef struct {
	GPIO_PinState previous_input;
	bool enabled;
	bool relay_state;
	uint32_t next_toggle;
} indicator_state_t;

void left_indicator_action();
void right_indicator_action();

// hazard
void hazard_action();

void relay_task(void * pvParameters);

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#endif /* INC_RELAY_H_ */
