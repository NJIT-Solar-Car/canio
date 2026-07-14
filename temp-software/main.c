#ifndef F_CPU
#define F_CPU 16000000UL  // 16 MHz external crystal on standard Nano
#endif

#include <avr/io.h>
#include <util/delay.h>

#define INDICATOR_LEFT	PORTD2
#define INDICATOR_RIGHT	PORTD3
#define HAZARD_LEFT		PORTD4
#define HAZARD_RIGHT	PORTD5
#define HORN			PORTD6

#define LEFT_INPUT		PORTC2
#define RIGHT_INPUT		PORTC3
#define HAZARD_INPUT	PORTC4
#define HORN_INPUT		PORTC5

uint8_t left_state, right_state, hazard_state, horn_state;

uint8_t getInput();

int main(void) {
    // Configure lights as outputs
    DDRD |= (1 << INDICATOR_LEFT) |
			(1 << INDICATOR_RIGHT) |
			(1 << HAZARD_LEFT) |
			(1 << HAZARD_RIGHT);

	
	// Configure GPIOs as inputs
	DDRC &= ~((1 << LEFT_INPUT) |
			(1 << RIGHT_INPUT) |
			(1 << HAZARD_INPUT));

    // turn all leds off
    PORTD &= ~((1 << INDICATOR_LEFT) |
				(1 << INDICATOR_RIGHT) |
				(1 << HAZARD_LEFT) |
				(1 << HAZARD_RIGHT));

    while (1) {
		switch (getInput()) {
			case 1:
				// Left indicator
				if (!left_state) {
					PORTD |= (1 << INDICATOR_LEFT);
					left_state = 1;
				} else {
					PORTD &= ~(1 << INDICATOR_LEFT);
					left_state = 0;
				}
			case 2:
				// Right indicator
				if (!right_state) {
					PORTD |= (1 << INDICATOR_RIGHT);
					right_state = 1;
				} else {
					PORTD &= ~(1 << INDICATOR_RIGHT);
					right_state = 0;
				}
			case 3:
				// Hazards
				if (!hazard_state) {
					PORTD |= (1 << HAZARD_LEFT) | (1 << HAZARD_RIGHT);
					hazard_state = 1;
				} else {
					PORTD &= ~((1 << HAZARD_LEFT) | (1 << HAZARD_RIGHT));
					hazard_state = 0;
				}
			case 4:
				// Horn
				if (!horn_state) {
					PORTD |= (1 << HORN);
					horn_state = 1;		
				}
			default:
				PORTD = 0x0;
				horn_state = 0;
				hazard_state = 0;
				left_state = 0;
				right_state = 0;
		}

        _delay_ms(250);
    }

    return 0; // Standard flow control requirement, never reached
}

uint8_t getInput() {
	// Naive code assumes one-hot situation type thing
	if (PINC & (1 << LEFT_INPUT)) {
		return 1;
	} else if (PINC & (1 << RIGHT_INPUT)) {
		return 2;
	} else if (PINC & (1 << HAZARD_INPUT)) {
		return 3;
	} else if (PINC & (1 << HORN)) {
		return 4;
	}
	return 0;
}
