#include "vlab.h"
#include "hc.h"
#include "utilities.h"
#include "ethernet.h"
#include "main.h"

void display(char* string) {
	uart_send_string(UART, string);
}

void display_char(char c) {
	uart_send_char(UART, c);
}

void inth_mac() {
	volatile int* packet;
	while ((packet = mac_packet_ready())) {
		short type = (short)((*(packet+3) & 0xFFFF0000) >> 16U);
		
		if (type == 0x55AA) {
			char dest        = (char)((*(packet+1) & 0x00FF0000) >> 16U);
			char source      = (char)((*(packet+2) & 0x000000FF));
			short stream     = (short)((*(packet+3) & 0x0000FFFF));
			char samplerate  = (char)((*(packet+4) & 0xFF000000) >> 24U);
			char samplewidth = (char)((*(packet+4) & 0x00FF0000) >> 16U);
			int index        = (int) (*(packet+5));
			int length       = (int) (*(packet+6));
			char data[length+3];
			
			for (int i=0; i*4 < length; i++) {
				unsigned int chunk = *(packet+7+i);
				data[i*4]   = (char)((chunk & 0xFF000000) >> 24U);
				data[i*4+1] = (char)((chunk & 0x00FF0000) >> 16U);
				data[i*4+2] = (char)((chunk & 0x0000FF00) >>  8U);
				data[i*4+3] = (char)((chunk & 0x000000FF));
			}
			
			mac_clear_rx_packet(packet);
			eth_rx_frame(dest, source, stream, samplerate, samplewidth, index, length, data);
		}
		else {
			mac_clear_rx_packet(packet);
		}
	}
}

void inth_uart() {
	while (uart_check_char(UART)) {
		uart_get_char(UART);
	}
}

void inth_switches() {	
	state.channel = (0x7 & get_switches()) + 1;
	state.rate    = 0;
	set_leds(1 << (state.channel-1));
	switches_clear_interrupt();
}

DECLARE_INTERRUPT_HANDLER(int_handler);
void int_handler() {
	int vec = intc_get_vector();
	
	switch (vec) {
		case INTC_MAC:      inth_mac();      break;
		case INTC_SWITCHES: inth_switches(); break;
		case INTC_UART:     inth_uart();     break;
	}
	
	intc_acknowledge_interrupt(vec);
}

int main(void) {
	initialise();
	
	// MAC
	eth_setup();
	intc_enable_interrupt(INTC_MAC);
	mac_enable_interrupts();
	
	// Switches
	intc_enable_interrupt(INTC_SWITCHES);
	switches_enable_interrupts();
	inth_switches(); // update leds to initial configuration
	
	// UART
	intc_enable_interrupt(INTC_UART);
	uart_enable_interrupts(UART);
	
	// Announce on network (so network switches learn device)
	eth_tx_announce();
	
	return 0;
}