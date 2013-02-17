#include "vlab.h"
#include "hc.h"

int hc_divide(int n, int d) {
	int r;
	putfslx(HC_OP_DIVIDE(n), 0, FSL_BLOCKING); // Send numerator
	putfslx(HC_OP_DIVIDE(d), 0, FSL_BLOCKING); // Send denominator.
	getfslx(r, 0, FSL_BLOCKING); // Get the result
	return HC_OPERAND(r);
}