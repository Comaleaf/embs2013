#include "vlab.h"
#include "hc.h"
#include "main.h"

void hc_put_char(unsigned char c) {
	HC_PUT(HC_8BIT, c);
}

void hc_put_short(short s) {
	HC_PUT(HC_16BIT, s);
}

void hc_set_mode(char rate, char width) {
	HC_PUT(HC_MODE, (rate<<4U) | width);
}