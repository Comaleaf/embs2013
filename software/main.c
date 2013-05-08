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

inline int get_channels() {
	return state.channels;
}

// Updates the state for which channels are active
// Works out what intervals are necessary
void set_channels(int new_channels) {
	char sampling_rate = 0;
	char digits[3];
	
	// If any packets were received whilst new channels are being set, the data sent
	// to the handel-c component would be sent without a preamble, so mac interrupts
	// are temporarily disabled.
	mac_disable_interrupts();

	state.channels = new_channels;      // Update state
	set_leds(0xFF & (new_channels>>1)); // Update LEDs on the Spartan

	uart_send_string(UART, "\r\n [Active channels:");
	
	// The highest rate that is a multiple of other rates must be found so that the sampling rate
	// can be set to it.
	for (int i=1; i<=20; i++) {
		if (TEST_BIT(state.channels, i)) {
			if (sampling_rate < channels[i-1].rate) {
				sampling_rate = channels[i-1].rate;
			}
			
			// Whilst checking the sample rates, also display the newly active channels to the user.
			uart_send_char(UART, ' ');
			uart_send_string(UART, int2digit(i, digits));
		}
	}
	uart_send_char(UART, ']');
	
	for (int i=1; i<=20; i++) {
		// The interval to insert samples for this channel into the buffer depends on what the sampling rate
		// will be set at.
		channels[i-1].interval = 1 << (sampling_rate - channels[i-1].rate);
	}
	
	// Reset the stream and set the buffer rate to the fastest.
	hc_reset_stream(sampling_rate);
		
	// Now packets can be safely forwarded to the handel-c component again.
	mac_enable_interrupts();
}

void inth_mac() {
	char digits[3];
	
	volatile int* packet;
	while ((packet = mac_packet_ready())) {
		short type = (short)((*(packet+3) & 0xFFFF0000) >> 16U);
		
		// Only handle packets with the correct type - all others should be ignored.
		if (type == 0x55AA) {
			short stream = (short)((*(packet+3) & 0x0000FFFF));
			
			// Get channel information and update database
			ChanInfo *channel = &channels[stream-1];
			channel->rate  = (char)((*(packet+4) & 0xFF000000) >> 24U) - 1;
			channel->width = (char)((*(packet+4) & 0x00FF0000) >> 16U) - 1;
			
			// Only forward packets for activated channels.
			if (TEST_BIT(state.channels, stream)) {				
				int index  = (int)(*(packet+5));
				int length = (int)(*(packet+6));
				
				// The preamble must be sent to the handel-c component first, so that it knows where to
				// position the samples in the buffer.
				hc_preamble(channel->width, channel->interval, (signed)state.last_index - index, length);
				
				// Print interval rate for debugging
				if (channel->interval != 1) {
				uart_send_string(UART, "\r\nInterval: ");
				uart_send_string(UART, int2digit(channel->interval, digits));
				}
				
				// Forward all packets to the handel-c component.
				for (int i=0; i < length/4; i++) {
					hc_put(*(packet+7+i));
				}
				
				// The position in the buffer where this packet ended must be stored so that the
				// following packet can be inserted in the correct relative position.
				state.last_index = index + (length * channel->interval);
			}
		}
		
		mac_clear_rx_packet(packet);
	}
}

void inth_uart() {
	while (uart_check_char(UART)) {
		// Forward any console input to the GUI module.
		gui_accept(uart_get_char(UART));
	}
}

void inth_switches() {
	// Set the active channel to be the switch just activated.
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