#ifndef __ETHERNET_H_
#define __ETHERNET_H_

void eth_setup();
void eth_tx_announce();
void eth_rx_frame(unsigned char dest, unsigned char source, short stream, char samplerate, char samplewidth, int index, int length, char* data);

#endif