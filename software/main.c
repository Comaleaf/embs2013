#include "vlab.h"
#include "hc.h"
#include "utilities.h"
#include "ethernet.h"
#include "gui.h"
#include "main.h"

struct {
	char should_reset;
	int channel_settings;
	int previous_index;
} state;

int get_channels() {
	return state.channel_settings;
}

void set_channels(int new_channels) {
	mac_disable_interrupts();

	state.should_reset = 1;
	state.channel_settings = new_channels;
	set_leds(0xFF & (new_channels>>1));

	uart_send_string(UART, "\r\n [Active Channels:");
	for (int i=1; i<=20; i++) {
		uart_send_char(UART, ' ');
		uart_send_char(UART, TEST_BIT(state.channel_settings, i) ? '1' : '0');
	}
	uart_send_char(UART, ']');
	
	mac_enable_interrupts();
}

void inth_mac() {
	volatile int* packet;
	while ((packet = mac_packet_ready())) {
		short type = (short)((*(packet+3) & 0xFFFF0000) >> 16U);
		
		if (type == 0x55AA) {
			short stream = (short)((*(packet+3) & 0x0000FFFF));
			char rate    = (char)((*(packet+4) & 0xFF000000) >> 24U);
			char width   = (char)((*(packet+4) & 0x00FF0000) >> 16U);
			int index    = (int)(*(packet+5));
			int length   = (int)(*(packet+6));
			
			if (TEST_BIT(state.channel_settings, stream)) {
				hc_new_packet(state.should_reset, width, rate, index - state.previous_index, length);
								
				for (int i=0; i < length/4; i++) {
					hc_put(*(packet+7+i));
				}
				
				state.previous_index = index;
				if (state.should_reset) {
					state.should_reset = 0;
				}
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
	set_channels(1<<((0x7 & get_switches()) + 1));
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
	state.should_reset = 1;
	
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