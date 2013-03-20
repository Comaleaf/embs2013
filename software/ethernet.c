#include "vlab.h"
#include "utilities.h"
#include "main.h"
#include "hc.h"
#include "ethernet.h"

void eth_setup() {
	// Set MAC address
	MAC_TX1[0] = 0x00112233;
	MAC_TX1[1] = 0x00160000;
	
	mac_set_mac_address();
}

void eth_tx_announce() {
	MAC_TX1[0] = 0xFFFFFFFF; // Destination MAC
	MAC_TX1[1] = 0xFFFF0011; // Destination MAC, Source MAC
	MAC_TX1[2] = 0x22330016; // Source MAC
	MAC_TX1[3] = 0x00000004; // Type, Length
	MAC_TX1[4] = 0xFFFFFFFF; // Data
	
	mac_send_packet(20);
}

void eth_rx_frame(unsigned char dest, unsigned char source, short stream, char samplerate, char samplewidth, int index, int length, char* data) {
	if (samplerate == 1 && samplewidth == 1 && index == 3) {
		while (length-- > 0) {
			hc_send_char(*(data++))
		}
	}
}