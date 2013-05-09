#include "utilities.h"

char* int2digit(int n, char *output) {
	if (n > 9) {
		output[0] = '0' + (n/10);
		output[1] = '0' + (n - (n/10)*10);
		output[2] = 0;
	}
	else {
		output[0] = '0' + n;
		output[1] = 0;
	}
	
	return output;
}