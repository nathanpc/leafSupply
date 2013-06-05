/**
 *	rotary_encoder.h
 *	A rotary encoder driver for the MSP430G2553.
 *
 *	@author Nathan Campos <nathanpc@dreamintech.net>
 */

#include <msp430g2553.h>
#include <stdint.h>

#include "boolean.h"
#include "rotary_encoder.h"
#include "delay.h"
#include "bitio.h"

// Define the rotary encoder pins.
#define A BIT7
#define B BIT7

/**
 * Check if the rotary encoder was rotated clockwise.
 * 
 * @return TRUE if it was rotated clockwise, FALSE if it was rotate counter clockwise.
 */
int8_t rotary_encoder_rotation() {
	/*if (((P1IN & A) == A) && ((P2IN & B) == 0)) {
		// Wait until the pulses finish doing their thing.
		while (((P1IN & A) == A) && ((P2IN & B) == 0)) {}
		while ((P2IN & B) == B) {}

		// It was rotated clockwise.
		return 1;
	} else if (((P2IN & B) == B) && ((P1IN & A) == 0)) {
		// Wait until the pulses finish doing their thing.
		while (((P2IN & B) == B) && ((P1IN & A) == 0)) {}
		while ((P1IN & A) == A) {}

		// It was rotated counter clockwise.
		return -1;
	}*/

	if ((P2IN & B) == B) {
		return -1;
	} else {
		return 1;
	}

	return 0;
}
