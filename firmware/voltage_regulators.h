/**
 *	voltage_regulators.h
 *	Voltage regulator control.
 *
 *	@author Nathan Campos <nathanpc@dreamintech.net>
 */

#ifndef _VOLTAGE_REGULATORS_H_
#define _VOLTAGE_REGULATORS_H_

#include "boolean.h"

#define LM317  0
#define VR7805 1
#define LD33V  2

typedef struct {
	unsigned int curr_onscreen;
	unsigned int state[3];

	char *list[3];
	float voltages[3];
} voltage_regulators;

voltage_regulators vregs;

void vreg_init();
void vreg_set_voltage(bool increment);
void toogle_power(unsigned char *shift_register, const unsigned int vreg);

#endif  // _VOLTAGE_REGULATORS_H_
