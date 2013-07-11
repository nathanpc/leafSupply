/**
 *	voltage_regulators.c
 *	Voltage regulator control.
 *
 *	@author Nathan Campos <nathanpc@dreamintech.net>
 */

#include <msp430g2553.h>
#include <stdint.h>

#include "voltage_regulators.h"
#include "74HC595.h"

#include "boolean.h"
#include "delay.h"
#include "bitio.h"

/**
 *	Initialize the variables.
 */
void vreg_init() {
	vregs.curr_onscreen = 0;

	vregs.state[LM317] = 0;
	vregs.state[VR7805] = 0;
	vregs.state[LD33V] = 0;

	vregs.list[LM317] = "LM317";
	vregs.list[VR7805] = "7805";
	vregs.list[LD33V] = "LD33V";

	vregs.voltages[LM317] = 1.2f;
	vregs.voltages[VR7805] = 5.0f;
	vregs.voltages[LD33V] = 3.37f;

	vregs.pwm[LM317] = 0;
	vregs.pwm[VR7805] = 0;
	vregs.pwm[LD33V] = 0;
}

/**
 *	Sets a voltage to the variable regulator.
 *
 *	@param increment Increment.
 */
void vreg_set_voltage(bool increment) {
	if (vregs.curr_onscreen == LM317) {
		if (increment) {
			// Assuming the PWM max is 100.
			if (vregs.pwm[LM317] < 100) {
				//vregs.voltages[LM317] += 0.1;
				vregs.pwm[LM317] += 1;
			}
		} else {
			// Assuming we won't go negative.
			if (vregs.pwm[LM317] > 0) {
				//vregs.voltages[LM317] -= 0.1;
				vregs.pwm[LM317] -= 1;
			}
		}

		if (vregs.state[LM317]) {
			TA1CCR1 = vregs.pwm[LM317];
		}
	}
}

void toggle_pwm(const unsigned int vreg) {
	if (vregs.state[vreg]) {
		TA1CCR1 = 0;
	} else {
		TA1CCR1 = vregs.pwm[vreg];
	}
}

/**
 *	Toggle a voltage regulator power.
 *
 *	@param shift_register A pointer to the shift register mask.
 *	@param vreg Desired voltage regulator to be toogled.
 */
void toggle_power(unsigned char *shift_register, const unsigned int vreg) {
	if (vreg == LM317) {
		toggle_pwm(vreg);
	}

	if (vregs.state[vreg]) {
		vregs.state[vreg] = 0;
	} else {
		vregs.state[vreg] = 1;
	}

	switch (vreg) {
		case 0:  // LM317
			*shift_register ^= 0b00000100;
			break;
		case 1:  // 7805
			*shift_register ^= 0b00000010;
			break;
		case 2:  // LD33V
			*shift_register ^= 0b00000001;
			break;
	}

	shift_out(*shift_register);
}