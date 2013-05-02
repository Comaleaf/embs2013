#ifndef __HC_H_
#define __HC_H_

void hc_reset_stream(char rate);
void hc_preamble(char width, char offset, char interval, int position, int length);
void hc_put(int data);

#endif