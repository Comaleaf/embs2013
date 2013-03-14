#ifndef __ETHERNET_H_
#define __ETHERNET_H_

void eth_setup();
void eth_rx_packet(unsigned char dest, unsigned char source, char* data, int length);
void eth_tx_string(unsigned char dest, char* data);
void eth_tx_packet(unsigned char dest, char* data, short length);

void eth_rx_frame(unsigned char dest, unsigned char source, short stream, char samplerate, char samplewidth, int index, int length, char* data);

#endif