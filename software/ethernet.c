#include "vlab.h"
#include "utilities.h"
#include "microblaze_0.h"
#include "ethernet.h"

void eth_setup() {
	*(MAC_TX1 + 0) = 0x00112233;
	*(MAC_TX1 + 1) = 0x00160000;
	mac_set_mac_address();
}

void eth_tx_string(char dest, char* data) {
	short length = 0;
	while (data[length] != 0) length++;
	
	eth_tx_packet(dest, data, length);
}

void eth_tx_packet(char dest, char* data, short length) {
	*(MAC_TX1+0) = 0x00112233;        //Destination MAC
	*(MAC_TX1+1) = 0x00000011|(0x00FF0000&(dest<<16)); //Destination MAC, Source MAC
	*(MAC_TX1+2) = 0x22330016;        //Source MAC
	*(MAC_TX1+3) = 0x55AA0000|length; //Type, Length
	strcopy(data, (char*)MAC_TX1+4, (int)length);
	mac_send_packet(16+length);
}

void eth_rx_packet(char dest, char source, char* data, int length) {
	char message[8+2+5+2+2+length+2+1];
	
	strcopy("\r\nFROM:## TO: ##\r\n", message, 18); 
	message[7]  = byte2hex((source & 0xF0) >> 4);
	message[8]  = byte2hex((source & 0x0F));
	message[14] = byte2hex((dest & 0xF0) >> 4);
	message[15] = byte2hex((dest & 0x0F));
	strcopy(data, &message[18], length);
	strcopy("\r\n", &message[18+length], 3);
		
	display(message);
}