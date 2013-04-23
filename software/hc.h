#ifndef __HC_H_
#define __HC_H_

void hc_new_packet(char reset, char width, char rate, int offset, int length);
void hc_put(int data);

#endif