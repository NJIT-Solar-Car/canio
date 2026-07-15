#ifndef F_CPU
#define F_CPU 16000000UL  // 16 MHz external crystal on standard Nano
#endif

#include <avr/io.h>
#include <util/delay.h>

#define INDICATOR_LEFT	PORTD2
#define INDICATOR_RIGHT	PORTD3
#define HORN_OUT		PORTD4
#define HAZARD_LEFT		PORTD5
#define HAZARD_RIGHT	PORTD6
#define HEADLIGHT_LEFT	PORTD7
#define HEADLIGHT_RIGHT	PORTB0

#define LEFT_INPUT		PORTC0
#define RIGHT_INPUT		PORTC1
#define HORN_INPUT		PORTC2
#define HAZARD_INPUT	PORTC3
#define HEADLIGHT_INPUT	PORTC4

uint8_t left_state = 0;
uint8_t right_state = 0;
uint8_t hazard_state = 0;
uint8_t horn_state = 0;
uint8_t headlight_state = 0;

void configure();

int main(void) {
	configure();

	while (1) {
		if (PINC & (1 << LEFT_INPUT)) {
			if (!left_state) {
				PORTD |= (1 << INDICATOR_LEFT);
				left_state = 1;
			} else {
				PORTD &= ~(1 << INDICATOR_LEFT);
				left_state = 0;
			}
		}

		if (PINC & (1 << RIGHT_INPUT)) {
			if (!right_state) {
				PORTD |= (1 << INDICATOR_RIGHT);
				right_state = 1;
			} else {
				PORTD &= ~(1 << INDICATOR_RIGHT);
				right_state = 0;
			}
		}

		if (PINC & (1 << HAZARD_INPUT)) {
			if (!hazard_state) {
				PORTD |= (1 << HAZARD_LEFT) | (1 << HAZARD_RIGHT);
				hazard_state = 1;
			} else {
				PORTD &= ~((1 << HAZARD_LEFT) | (1 << HAZARD_RIGHT));
				hazard_state = 0;
			}
		}

		if (PINC & (1 << HORN_INPUT)) {
			if (!horn_state) {
				PORTD |= (1 << HORN_OUT);
				horn_state = 1;		
			}
		}

		if (PINC & (1 << HEADLIGHT_INPUT)) {
			if (!headlight_state) {
				PORTD |= (1 << HEADLIGHT_LEFT);
				PORTB |= (1 << HEADLIGHT_RIGHT);
				headlight_state = 1;
			}
		}

		// Only wait if lights are on
        if (PORTD != 0x0 || PORTB != 0x0) {
			_delay_ms(250);
		}
	}

    return 0; // never reached
}

void configure() {
    // Configure outputs
    DDRD |= (1 << INDICATOR_LEFT) |
			(1 << INDICATOR_RIGHT) |
			(1 << HORN_OUT) |
			(1 << HAZARD_LEFT) |
			(1 << HAZARD_RIGHT) |
			(1 << HEADLIGHT_LEFT);

	DDRB |= (1 << HEADLIGHT_RIGHT);

	
	// Configure GPIOs as inputs
	DDRC &= ~((1 << LEFT_INPUT) |
			(1 << RIGHT_INPUT) |
			(1 << HORN_INPUT) |
			(1 << HAZARD_INPUT) |
			(1 << HEADLIGHT_INPUT));

    // turn all outputs off
    PORTD &= ~((1 << INDICATOR_LEFT) |
				(1 << INDICATOR_RIGHT) |
				(1 << HORN_OUT) |
				(1 << HAZARD_LEFT) |
				(1 << HAZARD_RIGHT) |
				(1 << HEADLIGHT_LEFT));

	PORTB &= ~(1 << HEADLIGHT_RIGHT);
}
