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

// Define the rotary encoder pins.
#define A BIT6
#define B BIT7

// Private function declarations.
unsigned int get_byte(char b, unsigned int pos);
void bit_to_pin(char c, unsigned int pos, unsigned int pin);


/**
 * Get a single bit from a byte.
 * 
 * @param b A byte.
 * @param pos The bit position to be extracted.
 * @return A bit.
 */
unsigned int get_byte(char b, unsigned int pos) {
	return (b & (1 << pos));
}

/**
 * Puts the desired bit into a pin. It's used to get the bits in a char
 * to send to the LCD.
 * 
 * @param c The character.
 * @param pos Bit position.
 * @param pin The pin to be set.
 */
void bit_to_pin(char c, unsigned int pos, unsigned int pin) {
	if (get_byte(c, pos)) {
		P1OUT |= pin;
	} else {
		P1OUT &= ~pin;
	}
}

/**
 * Setup the rotary encoder pins.
 */
void rotary_encoder_setup() {
	P1DIR |= (A + B);
}

/**
 * Check if the rotary encoder was rotated clockwise.
 * 
 * @return TRUE if it was rotated clockwise, FALSE if it was rotate counter clockwise.
 */
bool rotary_encoder_rotation() {
	if ((P1IN & A == A) && (P1IN & B == 0)) {
		// Wait until the pulses finish doing their thing.
		while ((P1IN & A == A) && (P1IN & B == 0)) {}
		while (P1IN & B == B) {}

		// It was rotated clockwise.
		return TRUE;
	} else if ((P1IN & B == B) && (P1IN & A == 0)) {
		// Wait until the pulses finish doing their thing.
		while ((P1IN & B == B) && (P1IN & A == 0)) {}
		while (P1IN & A == A) {}

		// It was rotated counter clockwise.
		return FALSE;
	}

	return NULL;
}
