#ifndef __HC_H_
#define __HC_H_

void hc_reset_stream(unsigned char buffer_8k_rate, unsigned char buffer_44k_rate);
void hc_preamble(unsigned char buffer, unsigned char width, unsigned char interval, short offset, unsigned short length);
void hc_put(int data);

#endif