#include "minivlab.h"
#include "hc.h"
#include "utilities.h"
#include "ethernet.h"
#include "gui.h"
#include "main.h"

ChanInfo channels[20];
char sample_rate_8k_buffer;
char sample_rate_44k_buffer;
	
struct {
	int channels;
} state;

inline int get_channels() {
	return state.channels;
}

// Updates the state for which channels are active
// Works out what intervals are necessary
void set_channels(int new_channels) {
	char digits[3];

	sample_rate_8k_buffer = 0;
	sample_rate_44k_buffer = 0;
	
	// If any packets were received whilst new channels are being set, the data sent
	// to the handel-c component would be sent without a preamble, so mac interrupts
	// are temporarily disabled.
	mac_disable_interrupts();

	state.channels = new_channels;      // Update state
	set_leds(0xFF & (new_channels>>1)); // Update LEDs on the Spartan

	uart_send_string(UART, "\r\n [Active channels:");
	
	// The highest rate that is a multiple of other rates must be found so that the sampling rate
	// can be set to it.
	for (int i=0; i<20; i++) {
		if (TEST_BIT(state.channels, i+1)) {
			// Determine which buffers are required
			if (channels[i].rate == RATE_8K) {
				sample_rate_8k_buffer = 1;
			}
			else {
				// And what rate to set the 44k buffer
				if (sample_rate_44k_buffer < channels[i].rate) {
					sample_rate_44k_buffer = channels[i].rate;
				}
			}
			
			// Whilst checking the sample rates, also display the newly active channels to the user.
			uart_send_char(UART, ' ');
			uart_send_string(UART, int2digit(i+1, digits));
		}
	}
	uart_send_char(UART, ']');
	
	for (int i=0; i<20; i++) {
		// The interval to insert samples for this channel into the buffer depends on what the sampling rate
		// will be set at.
		if (channels[i].rate == 0) {
			channels[i].interval = 1; // There are no multiples of 8K so always use interval 1
		}
		else {
			channels[i].interval = 1 << (sample_rate_44k_buffer - channels[i].rate);
		}
	}
	
	// Reset the stream and set the buffer rate to the fastest.
	hc_reset_stream(sample_rate_8k_buffer, sample_rate_44k_buffer);
		
	// Now packets can be safely forwarded to the handel-c component again.
	mac_enable_interrupts();
}

void inth_mac() {	
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
				if (channel->rate > 0) {
					hc_preamble(1, channel->width, channel->interval, index << (sample_rate_44k_buffer - channel->rate), length);
				}
				else {
					hc_preamble(0, channel->width, 1, index, length);
				}
				
				// Forward all packets to the handel-c component.
				for (int i=0; i < length/4; i++) {
					hc_put(*(packet+7+i));
				}
				
				// Now determine the length in ~samples~, not bytes.
				if (channel->width == WIDTH_16BIT) {
					length >>= 1; // Divide by two
				}
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
	if (TEST_BIT(get_switches(), 3)) {
		set_channels(1 << (get_switches()-3));
	}
	else {
		set_channels(1 << (get_switches()+1));
	}
	// Set the active channel to be the switch just activated.
	gui_prompt();
	
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
	gui_usage();
	gui_prompt();
	
	return 0;
}