#include "vlab.h"
#include "hc.h"

void hc_new_packet(char has_switched, char width, char rate, int index, int length) {
	int data;
	data = has_switched;
	data = (data<<1) + width-1;
	data = (data<<2) + rate-1;
	data = (data<<14) + index;
	data = (data<<14) + length;
	
	hc_put(data);
}

void hc_put(int data) {
	putfslx(data, 0, FSL_BLOCKING);
}