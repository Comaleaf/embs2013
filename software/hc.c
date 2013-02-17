#include "vlab.h"
#include "hc.h"

int hc_divide(int n, int d) {
	int r;
	putfslx(n, 0, FSL_BLOCKING); // Send numerator
	putfslx(d, 0, FSL_BLOCKING); // Send denominator.
	getfslx(r, 0, FSL_BLOCKING); // Get the result
	return r;
}