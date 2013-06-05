/**
 *	main.c
 *	leafSupply firmware for the MSP430G2553 microcontroller.
 *
 * @author Nathan Campos <nathanpc@dreamintech.net>
 */

#include <msp430g2553.h>
#include <stdio.h>
#include "boolean.h"
#include "delay.h"
#include "bitio.h"

#include "74HC595.h"
#include "rotary_encoder.h"
#include "HD44780.h"

// Interrupt pins.
#define BT_INT BIT3
#define RE_A_INT BIT7

unsigned char shift_default_on = 0b11111000;

int counter = 0; /////////

void setup_interrupts();
void handle_re_rotation();

void main() {
	WDTCTL = WDTPW + WDTHOLD;  // Stop watchdog timer.

	P1DIR &= ~(BT_INT + RE_A_INT);
	P2DIR &= ~RE_B_INT;
	P2SEL &= ~RE_B_INT;  // Turn XOUT into P2.7
	
	// Setup the interrupts.
	setup_interrupts();
	_BIS_SR(GIE);  // TODO: Do (LPMX + GIE) for low-power + interrupts.
	//__enable_interrupt();

	// Initialize the shift register driver.
	shift_register_setup();
	shift_out(shift_default_on);
	
	// Setup the LCD driver.
	lcd_init(TRUE, TRUE);
	
	lcd_print("Hello, World!");
	
	while (TRUE) {
	}
}

void setup_interrupts() {
	P1IES &= ~(BT_INT + RE_A_INT);  // Set the interrupt to be from LOW to HIGH.
	P1IFG &= ~(BT_INT + RE_A_INT);  // P1.3 and P1.7 IFG cleared
	P1IE |= (BT_INT + RE_A_INT);    // Set P1.3 and P1.7 as interrupt.

	//P2IES &= ~RE_B_INT;  // Set the interrupt to be from LOW to HIGH.
	//P2IFG &= ~RE_B_INT;  // P2.7 IFG cleared
	//P2IE |= RE_B_INT;    // Set P2.7 as interrupt.
}

void handle_bt_press(unsigned int bt) {
	switch (bt) {
		case 0:  // S_PWR
			lcd_return_home();
			lcd_print("S_PWR Pressed");
			break;
		case 1:  // S_MNU1
			lcd_return_home();
			lcd_print("S_MNU1 Pressed");
			break;
		case 2:  // S_MNU2
			lcd_return_home();
			lcd_print("S_MNU2 Pressed");
			break;
		case 3:  // S_RE
			lcd_return_home();
			lcd_print("S_RE Pressed");
			break;
		default:
			lcd_return_home();
			lcd_print("WTF you just pressed?!");
			break;
	}
}

void handle_re_rotation() {
	if (rotary_encoder_rotation() == 1) {
		counter++;
		lcd_return_home();
		//lcd_print("Clockwise");
		char t[10];
		sprintf(t,"%d",counter);
		lcd_return_home();
		lcd_print(t);
	} else if (rotary_encoder_rotation() == -1) {
		counter--;
		lcd_return_home();
		//lcd_print("Counter Clockwise");
		char t[10];
		sprintf(t,"%d",counter);
		lcd_return_home();
		lcd_print(t);
	}/* else {
		lcd_return_home();
		lcd_print("WTF?!");
	}*/
}

#pragma vector = PORT1_VECTOR
__interrupt void P1_ISR() {
	switch(P1IFG & (BT_INT + RE_A_INT)) {
		case BT_INT:
			// Handle the input array interrupt.
			P1IFG &= ~BT_INT;  // P1.3 IFG cleared.
			
			// Loops through the buttons.
			for (unsigned int i = 0; i < 4; i++) {
				unsigned char tmp[] = { 0b10000000, 0b01000000, 0b00100000, 0b00010000 };
				tmp[i] |= (shift_default_on & 0b00001111);

				// Check if the button was pressed.
				shift_out(tmp[i]);
				if ((P1IN & BT_INT) == BT_INT) {
					handle_bt_press(i);
				}
			}
			
			unsigned char lst = shift_default_on;
			//lst |= leds;
			shift_out(lst);
			
			while ((P1IN & BT_INT) == BT_INT) {}
			break;
		case RE_A_INT:
			// Handle the rotary encoder A interrupt.
			P1IFG &= ~RE_A_INT;  // P1.7 IFG cleared.
			handle_re_rotation();
		default:
			P1IFG = 0;
			break;
	}

	P1IFG = 0;
}