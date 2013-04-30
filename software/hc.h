#ifndef __HC_H_
#define __HC_H_

void hc_set_channels(int channels);
void hc_set_rate(char rate);
void hc_new_packet(char width, char offset, char interval, int position, int length);
void hc_put(int data);

#endif