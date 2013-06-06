/**
 *	ftoa.c
 *	Float to string implementation.
 *
 *	@author skumlerud <http://forum.arduino.cc/index.php?topic=44262.msg320447#msg320447>
 */

#include <stdlib.h>
#include <stdio.h>

#include "ftoa.h"

/**
 *	Converts a float to a string.
 *
 *	@param str A pointer to the final string.
 *	@param f The float.
 *	@param precision The precision.
 */
void ftoa(char *str, float f, const unsigned int precision) {
	long p[] = { 0, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000 };
	char *ret = str;
	long heiltal = (long)f;

	itoa(heiltal, str, 10);
	while (*str != '\0') {
		str++;
	}

	*str++ = '.';
	long desimal = abs((long)((f - heiltal) * p[precision]));
	itoa(desimal, str, 10);
}