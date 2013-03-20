#include "vlab.h"
#include "hc.h"
#include "main.h"

void hc_send_char(unsigned char c) {
	putfslx(HC_OP_SEND_CHAR(c), 0, FSL_BLOCKING);
}

void hc_send_short(short s) {
	putfslx(HC_OP_SEND_SHORT(s), 0, FSL_BLOCKING);	
}