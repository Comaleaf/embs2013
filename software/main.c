#include "vlab.h"
#include "hc.h"
#include "utilities.h"
#include "ethernet.h"
#include "gui.h"
#include "main.h"

ChanInfo channels[20];

struct {
	int channels;
	int last_index;
} state;

int get_channels() {
	return state.channels;
}

void set_channels(int new_channels) {
	char highest_rate = 0;
	char digits[3];
	
	mac_disable_interrupts();

	state.channels = new_channels;
	set_leds(0xFF & (new_channels>>1));

	uart_send_string(UART, "\r\n [Active channels:");
	for (int i=1; i<=20; i++) {
		if (TEST_BIT(state.channels, i)) {
			if (channels[i].rate > highest_rate) {
				highest_rate = channels[i].rate;
			}
			uart_send_char(UART, ' ');
			uart_send_string(UART, int2digit(i, digits));
		}
	}
	uart_send_char(UART, ']');
	
	for (int i=1; i<=20; i++) {
		channels[i].interval = 1 << (highest_rate - channels[i].rate);
	}
	
	// Reset the stream and set the buffer rate to the fastest
	hc_reset_stream(highest_rate);
	
	mac_enable_interrupts();
}

void inth_mac() {
	volatile int* packet;
	while ((packet = mac_packet_ready())) {
		short type = (short)((*(packet+3) & 0xFFFF0000) >> 16U);
		
		if (type == 0x55AA) {
			short stream = (short)((*(packet+3) & 0x0000FFFF));
			
			if (TEST_BIT(state.channels, stream)) {
				ChanInfo *channel = &channels[stream-1];
				
				channel->rate  = (char)((*(packet+4) & 0xFF000000) >> 24U);
				channel->width = (char)((*(packet+4) & 0x00FF0000) >> 16U);
				
				int index  = (int)(*(packet+5));
				int length = (int)(*(packet+6));
				
				hc_preamble(channel->width, channel->interval, (signed)state.last_index - index, length);
				
				for (int i=0; i < length/4; i++) {
					hc_put(*(packet+7+i));
				}
				
				state.last_index = index + length;
			}
		}
		
		mac_clear_rx_packet(packet);
	}
}

void inth_uart() {
	while (uart_check_char(UART)) {
		gui_accept(uart_get_char(UART));
	}
}

void inth_switches() {
	set_channels(1 << (get_switches()+1));
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
	
	// GUI prompt
	gui_prompt();
	
	return 0;
}