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
	display ("Rx\r\n");
	if (stream == state.channel) {
		if (samplerate != state.rate || samplewidth != state.width) {
			state.rate = samplerate;
			state.width = samplewidth;
			hc_set_mode(state.rate, state.width);
		}
		
		while (length > 0) {
			if (state.width == HC_8BIT) {
				hc_put_char(*(data++));
				length--;
			}
			else {
				hc_put_short(*(short*)data);
				data += 2;
				length -= 2;
			}
		}
	}
}