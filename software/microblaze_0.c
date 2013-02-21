#include "vlab.h"
#include "hc.h"
#include "utilities.h"
#include "ethernet.h"
#include "microblaze_0.h"

Operator op;
State state = STATE_MESSENGER;

char leds = 0;
int num1  = 0;
int num2  = 0;

char output[11] = {0};

void enable_calculator() {
	display("\r\nCalculator:\r\n\t");
	state = STATE_NUM_1;
}

void enable_messenger() {
	display("\r\nMessenger:\r\n");
	state = STATE_MESSENGER;
}

void display(char* string) {
	uart_send_string(UART, string);
}

void display_char(char c) {
	uart_send_char(UART, c);
}

State state_num_1(char c) {
	if (IS_DIGIT(c)) {
		num1 = num1*10 + (c-48);
		display_char(c);
	}
	else {
		switch (c) {
			case 42: op = MULT;  display_char(' ');display_char(c);display_char(' '); return STATE_NUM_2;
			case 43: op = PLUS;  display_char(' ');display_char(c);display_char(' '); return STATE_NUM_2;
			case 45: op = MINUS; display_char(' ');display_char(c);display_char(' '); return STATE_NUM_2;
			case 47: op = DIV;   display_char(' ');display_char(c);display_char(' '); return STATE_NUM_2;
		}
	}
	return STATE_NUM_1;
}

State state_num_2(char c) {
	if (IS_DIGIT(c)) {
		num2 = num2*10 + (c-48);
		display_char(c);
	}
	else if (c == 61) { // '='
		switch (op) {
			case PLUS:  asciify(num1+num2, 10, output); break;
			case MINUS: asciify(num1-num2, 10, output); break;
			case MULT:  asciify(num1*num2, 10, output); break;
			case DIV:   asciify(hc_divide(num1,num2), 10, output); break;
		}
		
		display(" = \n");
		display(output);
		display(".\r\n\t");

		num1 = 0;
		num2 = 0;
		return STATE_NUM_1;
	}
	
	return STATE_NUM_2;
}

void write_leds(char c) {
	leds = c;
	set_leds(c);
}

void inth_mac() {
	volatile int* buffer;
	while ((buffer = mac_packet_ready())) {
		short type   = (short)((*(buffer+3) & 0xFFFF0000) >> 16);

		if (type == 0x55AA || state != STATE_MESSENGER) {
			char dest    = (char)((*(buffer+1) & 0x00FF0000) >> 16);
			char source  = (char)(*(buffer+2) & 0x000000FF);
			short length = (short)(*(buffer+3) & 0x0000FFFF);
			char data[length+3];

			short chunk = 0;
			while (chunk*4 < length) {
				data[chunk*4]   = (char)((*(buffer+4+chunk) & 0xFF000000) >> 24);
				data[chunk*4+1] = (char)((*(buffer+4+chunk) & 0x00FF0000) >> 16);
				data[chunk*4+2] = (char)((*(buffer+4+chunk) & 0x0000FF00) >> 8);
				data[chunk*4+3] = (char)((*(buffer+4+chunk) & 0x000000FF));
				
				chunk++;
			}
		
			mac_clear_rx_packet(buffer);
			eth_rx_packet(dest, source, data, length);
		}
		else {
			mac_clear_rx_packet(buffer);
		}
	}
}

void inth_uart() {
	while (uart_check_char(UART)) {
		switch (state) {
			case STATE_NUM_1: state = state_num_1(uart_get_char(UART)); break;
			case STATE_NUM_2: state = state_num_2(uart_get_char(UART)); break;
			case STATE_MESSENGER: break;
			case STATE_COMPOSER: break;
		}
	}
}

void inth_switches() {
	char switches = get_switches();
	
	if (TEST_BIT(switches, 0)) {
		if (state != STATE_NUM_1 && state != STATE_NUM_2) {
			enable_calculator();
		}
	}
	else {
		if (state != STATE_MESSENGER && state != STATE_COMPOSER) {
			enable_messenger();
		}
	}
	
	write_leds((switches << 4) | (leds & 0x0F));
	
	switches_clear_interrupt();
}

void inth_buttons() {
	char buttons = get_buttons();
	
	eth_tx_string(0x00, "Hello 0x10!!!");
	
	if      (TEST_BIT(buttons, 0)) write_leds(leds & 0xF0);
	else if (TEST_BIT(buttons, 1)) write_leds(leds ^ 8);
	else if (TEST_BIT(buttons, 2)) write_leds(leds ^ 4);
	else if (TEST_BIT(buttons, 3)) write_leds(leds ^ 2);
	else if (TEST_BIT(buttons, 4)) write_leds(leds ^ 1);
	
	buttons_clear_interrupt();
}

void inth_fsl() {
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
		case INTC_FSL:      inth_fsl();      break;
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
	//intc_enable_interrupt(INTC_FSL);
	
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
	 
	eth_tx_string(0xFF, "Hello from Lauren!!!!");
	
	return 0;
}