#include "vlab.h"
#include "hc.h"

void hc_reset_stream(unsigned char buffer_8k_rate, unsigned char buffer_44k_rate) {
	hc_put(0x80000000 | ((buffer_8k_rate<<2) & 0x4) | (buffer_44k_rate & 0x3));
}

void hc_preamble(unsigned char buffer, unsigned char width, unsigned char interval, short offset, unsigned short length) {
	unsigned int data;
	
	offset <<= 2;
	
	data = buffer & 0x1;
	data = (data<<1) + (width & 0x1);
	data = (data<<3) + (interval & 0xF);
	data = (data<<16) + ((unsigned)offset);
	data = (data<<10) + (length & 0x3FF);
	
	hc_put(data);
}

inline void hc_put(int data) {
	putfslx(data, 0, FSL_BLOCKING);
}