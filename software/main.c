#include "vlab.h"
#include "hc.h"
#include "utilities.h"
#include "ethernet.h"
#include "main.h"

char leds = 0;

void display(char* string) {
	uart_send_string(UART, string);
}

void display_char(char c) {
	uart_send_char(UART, c);
}

void write_leds(char c) {
	leds = c;
	set_leds(c);
}

void inth_mac() {
	static int bufCursor = 0;
	
	volatile int* packet;
	while ((packet = mac_packet_ready())) {
		short type   = (short)((*(packet+3) & 0xFFFF0000) >> 16U);
		
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
	char switches = get_switches();
	
	write_leds((switches << 4) | (leds & 0x0F));
	
	switches_clear_interrupt();
}

void inth_buttons() {
	char buttons = get_buttons();
	
	if      (TEST_BIT(buttons, 0)) write_leds(leds & 0xF0);
	else if (TEST_BIT(buttons, 1)) write_leds(leds ^ 8);
	else if (TEST_BIT(buttons, 2)) write_leds(leds ^ 4);
	else if (TEST_BIT(buttons, 3)) write_leds(leds ^ 2);
	else if (TEST_BIT(buttons, 4)) write_leds(leds ^ 1);
	
	buttons_clear_interrupt();
}

void inth_fsl() {
	char output[10] = {0};
	int rv;
	getfslx(rv, 0, FSL_BLOCKING);
	asciify(rv, 10, output);
	display(output);
}

DECLARE_INTERRUPT_HANDLER(int_handler);
void int_handler() {
	int vec = intc_get_vector();
	
	switch(vec) {
		case INTC_MAC:      inth_mac();      break;
		case INTC_BUTTONS:  inth_buttons();  break;
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
	
	// FSL
	intc_enable_interrupt(INTC_FSL);
	
	// Buttons
	intc_enable_interrupt(INTC_BUTTONS);
	buttons_enable_interrupts();
	
	// Switches
	intc_enable_interrupt(INTC_SWITCHES);
	switches_enable_interrupts();
	inth_switches(); // update leds to initial configuration
	
	// UART
	intc_enable_interrupt(INTC_UART);
	uart_enable_interrupts(UART);
	
	// Announce on network (so switches display broadcasts)
	eth_tx_string(0xFF, "Announcing (Hello!)");
	
	return 0;
}