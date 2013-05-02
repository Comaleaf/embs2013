#include "vlab.h"
#include "hc.h"

void hc_reset_stream(char rate) {
	hc_put(0x80000000 | (rate & 0x3));
}

void hc_preamble(char width, char interval, int offset, int length) {
	unsigned int data;
	
	data = width & 0x1;
	data = (data<<5) + (interval & 0x1F);
	data = (data<<15) + (offset & 0x3FF);
	data = (data<<10) + (length & 0x3FF);
	
	hc_put(data);
}

inline void hc_put(int data) {
	putfslx(data, 0, FSL_BLOCKING);
}