/**
 *	main.c
 *	leafSupply firmware for the MSP430G2553 microcontroller.
 *
 * @author Nathan Campos <nathanpc@dreamintech.net>
 */

#include <msp430g2553.h>
#include <stdio.h>
#include <string.h>

#include "boolean.h"
#include "delay.h"
#include "bitio.h"
#include "ftoa.h"

#include "74HC595.h"
#include "rotary_encoder.h"
#include "HD44780.h"
#include "voltage_regulators.h"

// Interrupt pins.
#define BT_INT   BIT3
#define RE_A_INT BIT7
#define RE_B_INT BIT7

// Screen codes.
#define HOME_SCREEN 0

unsigned char shift_default_on = 0b11111000;

void print_voltage();
void print_vreg_state();
void print_screen(const unsigned int title);
void switch_vreg();

void setup_interrupts();
void handle_re_rotation();
void handle_bt_press(unsigned int bt);

/**
 *	Where the magic starts.
 */
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

	// Initilalize the voltage regulator driver.
	vreg_init();

	// Go to the home screen.
	print_screen(HOME_SCREEN);
	
	while (TRUE) {
	}
}

/**
 *	Setup the interrupts stuff.
 */
void setup_interrupts() {
	P1IES &= ~(BT_INT + RE_A_INT);  // Set the interrupt to be from LOW to HIGH.
	P1IFG &= ~(BT_INT + RE_A_INT);  // P1.3 and P1.7 IFG cleared
	P1IE |= (BT_INT + RE_A_INT);    // Set P1.3 and P1.7 as interrupt.

	//P2IES &= ~RE_B_INT;  // Set the interrupt to be from LOW to HIGH.
	//P2IFG &= ~RE_B_INT;  // P2.7 IFG cleared
	//P2IE |= RE_B_INT;    // Set P2.7 as interrupt.
}

/**
 *	Prints a "screen".
 *
 *	@param title The screen you want to display.
 */
void print_screen(const unsigned int title) {
	lcd_clear();

	switch (title) {
		case HOME_SCREEN:
			// +----------------+
			// |LM317       7.8V|
			// |             OFF|
			// +----------------+
			lcd_print(vregs.list[vregs.curr_onscreen], 0, 0);
			print_voltage();
			print_vreg_state();
			break;
	}
}

/**
 *	Cycles between the available voltage regulators.
 */
void switch_vreg() {
	if (vregs.curr_onscreen < 2) {
		vregs.curr_onscreen++;
	} else  {
		vregs.curr_onscreen = 0;
	}

	print_screen(HOME_SCREEN);
}

/**
 *	Prints the current voltage regulator state. (ON/OFF)
 */
void print_vreg_state() {
	if (vregs.state[vregs.curr_onscreen]) {
		lcd_print(" ON", 1, 13);
	} else {
		lcd_print("OFF", 1, 13);
	}
}

/**
 *	Prints the voltage that should be going out of the voltage regulator.
 */
void print_voltage() {
	char vstr[10];
	ftoa(vstr, vregs.voltages[vregs.curr_onscreen], 1);
	strcat(vstr, "V");
	//sprintf(vstr, "%4.2fV", );
	lcd_print(vstr, 0, 16 - strlen(vstr));
}


//
//	Handlers
//

/**
 *	Handles a button press.
 *
 *	@param bt A button.
 */
void handle_bt_press(unsigned int bt) {
	switch (bt) {
		case 0:  // S_PWR
			lcd_print("S_PWR Pressed", 1, 0);
			break;
		case 1:  // S_MNU1
			// Change voltage regulator.
			//lcd_print("S_MNU1 Pressed", 1, 0);
			switch_vreg();
			break;
		case 2:  // S_SEL
			lcd_print("S_SEL Pressed", 1, 0);
			break;
		case 3:  // S_MNU2
			// Toogle ON/OFF
			//lcd_print("S_MNU2 Pressed", 1, 0);
			toogle_power(&shift_default_on, vregs.curr_onscreen);
			print_vreg_state();
			break;
		default:
			lcd_print("WTF you just pressed?!", 1, 0);
			break;
	}
}

/**
 *	Handles a rotary encoder rotation.
 */
void handle_re_rotation() {
	if (rotary_encoder_rotation() == 1) {
		vreg_set_voltage(TRUE);
		print_voltage();
	} else if (rotary_encoder_rotation() == -1) {
		vreg_set_voltage(FALSE);
		print_voltage();
	}
}


//
//	Interrupts
//

/**
 *	Interrupt service routine for P1.
 */
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
