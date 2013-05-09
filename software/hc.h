#ifndef __HC_H_
#define __HC_H_

void hc_reset_stream(char buffer_8k_rate, char buffer_44k_rate);
void hc_preamble(char width, char interval, short offset, unsigned short length);
void hc_put(int data);

#endif