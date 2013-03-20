#include "vlab.h"
#include "hc.h"
#include "main.h"

void hc_send_char(unsigned char c) {
	HC_PUT(HC_8BIT, c);
}

void hc_send_short(short s) {
	HC_PUT(HC_16BIT, s);
}

void hc_clear_buffer() {
	HC_PUT(HC_CLEAR);
}