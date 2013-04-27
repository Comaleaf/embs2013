#include "vlab.h"
#include "hc.h"

void hc_set_channels(int channels) {
	hc_put(0x80000000 | channels);
}

void hc_new_packet(char reset, char width, char rate, int offset, int length) {
	int data;
	
	data = reset & 0x1;
	data = (data<<1) + ((width-1) & 0x1);
	data = (data<<2) + ((rate-1) & 0x3);
	data = (data<<14) + (offset & 0x3FFF);
	data = (data<<14) + (length & 0x3FFF);

	hc_put(data);
}

void hc_put(int data) {
	putfslx(data, 0, FSL_BLOCKING);
}