#include "vlab.h"
#include "utilities.h"
#include "main.h"
#include "ethernet.h"

char* sender(unsigned char source) {
	switch (source) {
		case 0x00: return "CatFact";
		case 0x01: return "ddjb500";
		case 0x02: return "jjb513 ";
		case 0x03: return "mb798  ";
		case 0x04: return "dc671  ";
		case 0x05: return "lc758  ";
		case 0x06: return "mc685  ";
		case 0x07: return "jbc504 ";
		case 0x08: return "tod502 ";
		case 0x09: return "re588  ";
		case 0x0A: return "af590  ";
		case 0x0B: return "af582  ";
		case 0x0C: return "ajf518 ";
		case 0x0D: return "pg591  ";
		case 0x0E: return "pg562  ";
		case 0x0F: return "di508  ";
		case 0x10: return "rhwm501";
		case 0x11: return "mbm503 ";
		case 0x12: return "lm638  ";
		case 0x13: return "cr640  ";
		case 0x14: return "ms882  ";
		case 0x15: return "dt620  ";
		case 0x16: return "lt696  ";
		case 0x17: return "ptn500 ";
		case 0x18: return "rt615  ";
		case 0x19: return "mbw500 ";
		case 0xFF: return "* (All)";
	}
	
	char* rtn =   "(##)   ";
	rtn[1] = byte2hex((source & 0xF0) >> 4);
	rtn[2] = byte2hex((source & 0x0F));
	return rtn;
}

void eth_setup() {
	// Set MAC address
	MAC_TX1[0] = 0x00112233;
	MAC_TX1[1] = 0x00160000;
	mac_set_mac_address();
}

void eth_tx_string(unsigned char dest, char* data) {
	short length = 0;
	while (data[length] != 0) length++;
	
	eth_tx_packet(dest, data, length);
}

void eth_tx_packet(unsigned char dest, char* data, short length) {	
	MAC_TX1[2] = 0x22330016; // Source MAC
	MAC_TX1[3] = 0x55AA0000|length; // Type, Length
	
	if (dest == 0xFF) {
		MAC_TX1[0] = 0xFFFFFFFF; // Destination MAC
		MAC_TX1[1] = 0xFFFF0011; // Destination MAC, Source MAC
	}
	else {
		MAC_TX1[0] = 0x00112233; // Destination MAC
		MAC_TX1[1] = 0x00000011|(0x00FF0000&(dest<<16)); // Destination MAC, Source MAC	
	}
		
	for (int i=0; i*4<length; i++) {
		MAC_TX1[4+i] = ((data[4*i]<<24) + (data[4*i+1]<<16) + (data[4*i+2]<<8) + (data[4*i+3]));
	}
	
	mac_send_packet(16+length);
}

void eth_rx_packet(unsigned char dest, unsigned char source, char* data, int length) {
	char message[25+length];

	strncpy(message+0,  "\r\n####### -> #######\r\n", 22);
	strncpy(message+2,  sender(source), 7);
	strncpy(message+13, sender(dest), 7);
	strncpy(message+22, data, length);
	strncpy(message+22+length, "\r\n", 3);

	display(message);
}