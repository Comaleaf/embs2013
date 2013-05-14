#include "vlab.h"
#include "utilities.h"
#include "hc.h"

inline void hc_sync() {
	hc_put(0xFFFFFFFF);
	hc_put(0x00000000);
	hc_put(0xFFFFFFFF);
}

void hc_reset_stream(unsigned char buffer_8k_rate, unsigned char buffer_44k_rate) {
	hc_sync();
	hc_put(0x80000000 | ((buffer_8k_rate<<2) & 0x4) | (buffer_44k_rate & 0x3));
}

void hc_preamble(unsigned char buffer, unsigned char width, unsigned char interval, unsigned short position, unsigned short length) {
	unsigned int data;
	
	/*if (offset < 0) {
		uart_send_char(UART, '-');
	}
	else {
		uart_send_char(UART, '+');
	}*/
	
	/*
	char digits[3];
	uart_send_string(UART, "\r\n");
	uart_send_string(UART, int2digit(buffer, digits));
	uart_send_char(UART, ' ');
	uart_send_string(UART, int2digit(width, digits));
	uart_send_char(UART, ' ');
	uart_send_string(UART, int2digit(interval, digits));*/

	data = 0;                               // Reset flag
	data = data + (buffer & 0x1);           // Buffer flag
	data = (data<<1) + (width & 0x1);       // Width flag
	data = (data<<3) + (interval & 0xF);    // Interval
	data = (data<<13) + (position & 0x1FFF); // Ofset
	data = (data<<13) + (length & 0x1FFF);   // Length
	
	hc_sync();
	hc_put(data);
}

inline void hc_put(int data) {
	putfslx(data, 0, FSL_BLOCKING);
}