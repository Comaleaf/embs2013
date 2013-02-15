#include "vlab.h"
#include "hc.h"

void hc_setstate(State state) {
	putfslx(state, 0, FSL_BLOCKING);
}

void hc_print(char *output) {
	hc_setstate(HC_STATE_PRINT);
	
	for (;;) {
		putfslx(*output, 0, FSL_BLOCKING);
		if (!*output) return;
		output++;
	}
}