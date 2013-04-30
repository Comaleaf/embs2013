#include "vlab.h"
#include "hc.h"
#include "utilities.h"
#include "ethernet.h"
#include "gui.h"
#include "main.h"

ChanInfo channels[20];

struct {
	char reset;
	int channels;
	int last_index[20];
} state;

int get_channels() {
	return state.channels;
}

void set_channels(int new_channels) {
	short num_active_channels;
	
	mac_disable_interrupts();

	state.reset = 1;
	state.channels = new_channels;
	set_leds(0xFF & (new_channels>>1));

	uart_send_string(UART, "\r\n [Active channels:");
	for (int i=1; i<=20; i++) {
		uart_send_char(UART, ' ');
		if (TEST_BIT(state.channels, 1)) {
			channels[i].offset = num_active_channels++;
			channels[i].interval = channels[i].rate;
			uart_send_char(UART, '1');
		}
		else {
			uart_send_char(UART, '0');
		}	
	}
	uart_send_char(UART, ']');
	
	for (int i=1; i<=20; i++) {
		channels[i].interval *= num_active_channels;
	}
	
	mac_enable_interrupts();
}

void inth_mac() {
	volatile int* packet;
	while ((packet = mac_packet_ready())) {
		short type = (short)((*(packet+3) & 0xFFFF0000) >> 16U);
		
		if (type == 0x55AA) {
			short stream = (short)((*(packet+3) & 0x0000FFFF));
			
			if (TEST_BIT(state.channels, stream)) {
				channels[stream].rate  = (char)((*(packet+4) & 0xFF000000) >> 24U);
				channels[stream].width = (char)((*(packet+4) & 0x00FF0000) >> 16U);
				
				int index  = (int)(*(packet+5));
				int length = (int)(*(packet+6));
				
				hc_new_packet(state.reset, channels[stream].width, channels[stream].rate, index - state.last_index[stream-1], length);
				
				for (int i=0; i < length/4; i++) {
					hc_put(*(packet+7+i));
				}
				
				state.last_index[stream-1] = index;
				if (state.reset) {
					state.reset = 0;
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
	set_channels(1<<(get_switches() + 1));
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
	state.reset = 1;
	
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