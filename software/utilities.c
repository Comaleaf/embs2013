#include "utilities.h"

// Produces the ASCII numeral string for a 1 or 2 digit number.
char* int2digit(int n, char *output) {
	// If there's more than one character.
	if (n > 9) {
		// Fill two characters.
		output[0] = '0' + (n/10);
		output[1] = '0' + (n - (n/10)*10);
		output[2] = 0;
	}
	else {
		// Otherwise fill one character.
		output[0] = '0' + n;
		output[1] = 0;
	}
	
	return output;
}