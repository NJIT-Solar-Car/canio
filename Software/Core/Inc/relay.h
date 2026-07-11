/*
 * relay.h
 *
 *  Created on: Jul 11, 2026
 *      Author: marco
 */

#ifndef INC_RELAY_H_
#define INC_RELAY_H_

#include "main.h"

typedef struct {
	GPIO_TypeDef *input_port;
	uint16_t input_pin;

	GPIO_TypeDef *output_port;
	uint16_t output_pin;
} relay_def_t;

static const relay_def_t relay_definitions[] = {
	{GPIO0_GPIO_Port, GPIO0_Pin, HighChan0_GPIO_Port, HighChan0_Pin},
	{GPIO1_GPIO_Port, GPIO1_Pin, HighChan1_GPIO_Port, HighChan1_Pin},
	{GPIO2_GPIO_Port, GPIO2_Pin, HighChan2_GPIO_Port, HighChan2_Pin},
	{GPIO3_GPIO_Port, GPIO3_Pin, HighChan3_GPIO_Port, HighChan3_Pin},
	{GPIO4_GPIO_Port, GPIO4_Pin, HighChan4_GPIO_Port, HighChan4_Pin},
	{GPIO5_GPIO_Port, GPIO5_Pin, HighChan5_GPIO_Port, HighChan5_Pin},
	{GPIO6_GPIO_Port, GPIO6_Pin, HighChan6_GPIO_Port, HighChan6_Pin},
};

void relay_task();

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#endif /* INC_RELAY_H_ */
