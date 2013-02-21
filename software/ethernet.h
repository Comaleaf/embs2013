#ifndef __ETHERNET_H_
#define __ETHERNET_H_

void eth_setup();
void eth_rx_packet(char dest, char source, char* data, int length);
void eth_tx_string(char dest, char* data);
void eth_tx_packet(char dest, char* data, short length);

#endif