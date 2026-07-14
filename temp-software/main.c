#ifndef F_CPU
#define F_CPU 16000000UL  // 16 MHz external crystal on standard Nano
#endif

#include <avr/io.h>
#include <util/delay.h>

#define INDICATOR_LEFT	PD2
#define INDICATOR_RIGHT	PD3
#define HAZARD_LEFT		PD4
#define HAZARD_RIGHT	PD5

#define LEFT_INPUT		PC2
#define RIGHT_INPUT		PC3
#define HAZARD_INPUT	PC4

uint8_t left_state, right_state, hazard_state;

uint8_t getInput();

int main(void) {
    // Configure lights as outputs
    DDRD |= (1 << INDICATOR_LEFT) |
			(1 << INDICATOR_RIGHT) |
			(1 << HAZARD_LEFT) |
			(1 << HAZARD_RIGHT);

	
	// Configure GPIOs as inputs
	DDRD &= (1 << LEFT_INPUT) |
			(1 << RIGHT_INPUT) |
			(1 << HAZARD_INPUT);

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
		}

        _delay_ms(250);
    }

    return 0; // Standard flow control requirement, never reached
}

uint8_t getInput() {
	// Naive code assumes one-hot situation type thing
	if (PIND & (1 << LEFT_INPUT)) {
		return 1;
	} else if (PIND & (1 << RIGHT_INPUT)) {
		return 2;
	} else if (PIND & (1 << HAZARD_INPUT)) {
		return 3;
	}
	return 0;
}
