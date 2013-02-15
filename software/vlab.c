#include "vlab.h"

int shadow_interrupt_enable;

void initialise(void) {
	//Implement a delay to allow other hardware to start up
	volatile int i;
	for(i = 0; i < 800; i++) i = i;
	
	shadow_interrupt_enable = 0;
	
	intc_master_enable();
	mb_enable_interrupts();
}

// UART
//------------------------------------------------------------
void uart_send_char(volatile int *uart, char c) {
	while (*(uart + 2) & 0x08);
	*(uart + 1) = c;
}

void uart_send_string(volatile int *uart, char *string) {
	int x = 0;
	while(string[x] != '\0') {
		uart_send_char(uart, string[x]);
		x++;
	}
}

char uart_get_char (volatile int *uart) {
	return (char)*uart;
}

int uart_check_char(volatile int *uart) {
	return (*(uart + 2) & 0x01);
}

char uart_wait_char(volatile int *uart) {
	while (!(*(uart + 2) & 0x01));
	return (char)*uart;
}

char byte2hexchar(int i) {
	if (i < 10) return i + 48;
	else return (i-10) + 65;
}

void word2asciihex(int i, char* buf) {
	buf[0] = byte2hexchar((i & 0xF0000000) >> 28);
	buf[1] = byte2hexchar((i & 0x0F000000) >> 24);
	buf[2] = byte2hexchar((i & 0x00F00000) >> 20);
	buf[3] = byte2hexchar((i & 0x000F0000) >> 16);
	buf[4] = byte2hexchar((i & 0x0000F000) >> 12);
	buf[5] = byte2hexchar((i & 0x00000F00) >> 8);
	buf[6] = byte2hexchar((i & 0x000000F0) >> 4);
	buf[7] = byte2hexchar((i & 0x0000000F));
	buf[8] = '\0';
}

void uart_print_hex_8(volatile int *uart, char c) {
	uart_send_char(uart, byte2hexchar((c & 0xF0) >> 4));
	uart_send_char(uart, byte2hexchar((c & 0x0F)));
}

void uart_print_hex_16(volatile int *uart, short c) {
	uart_send_char(uart, byte2hexchar((c & 0xF000) >> 12));
	uart_send_char(uart, byte2hexchar((c & 0x0F00) >> 8));
	uart_send_char(uart, byte2hexchar((c & 0x00F0) >> 4));
	uart_send_char(uart, byte2hexchar((c & 0x000F)));
}

void uart_print_hex_32(volatile int *uart, int i) {
	char temp[9];
	word2asciihex(i, temp);
	uart_send_string(uart, temp);
}

void uart_enable_interrupts(volatile int *uart) {
	*(uart + 3) = 0x10;
}

void uart_disable_interrupts(volatile int *uart) {
	*(uart + 3) = 0;
}



//Basic
//------------------------------------------------------------
void set_leds(char c) {
	*(LEDS) = c;
}

char get_buttons() {
	return *(BUTTONS);
}

char get_switches() {
	return *(SWITCHES);
}

void switches_enable_interrupts() {
	*(SWITCHES+71) = 0x80000001;
	*(SWITCHES+74) = 0x00000001;
}

void switches_disable_interrupts() {
	*(SWITCHES+71) = 0x00000000;
	*(SWITCHES+74) = 0x00000000;
}

void switches_clear_interrupt() {
	*(SWITCHES+72) = 0x80000001;
}

void buttons_enable_interrupts() {
	*(BUTTONS+71) = 0x80000001;
	*(BUTTONS+74) = 0x00000001;
}

void buttons_disable_interrupts() {
	*(BUTTONS+71) = 0x00000000;
	*(BUTTONS+74) = 0x00000000;
}

void buttons_clear_interrupt() {
	*(BUTTONS+72) = 0x80000001;
}


//Interrupts
//------------------------------------------------------------

void intc_enable_all_interrupts() {
	(*(INTC + 7)) = 0xFF; //Master enable
	(*(INTC + 2)) = 0xFF;
}

void intc_disable_all_interrupts() {
	(*(INTC + 2)) = 0x00;
}

void intc_enable_interrupt(int num) {
	int i, mask;
	mask = 0x01;
	for(i = 0; i < num; i++) mask = mask << 1;
	(*(INTC + 4)) = mask;
}

void intc_disable_interrupt(int num) {
	int i, mask;
	mask = 0x01;
	for(i = 0; i < num; i++) mask = mask << 1;
	(*(INTC + 5)) = mask;
}

void intc_acknowledge_interrupt(int num) {
	int i, mask;
	mask = 0x01;
	for(i = 0; i < num; i++) mask = mask << 1;
	(*(INTC + 3)) = mask;
}

void intc_acknowledge_all() {
	(*(INTC + 3)) = 0xFF;
}

void intc_master_enable() {
	(*(INTC + 7)) = 0xFF;
}

int intc_get_vector() {
	return (*(INTC + 6));
}

//Graphics
//------------------------------------------------------------

void gfx_set_frame_base_address(int addr) {
	*(VGA) = addr;
};

void gfx_set_palette(unsigned char index, unsigned char r, unsigned char g, unsigned char b) {
	*((VGA + 1)) = (index << 16) | ((b & 0x1F) << 10) | ((g & 0x1F) << 5) | (r & 0x1F);
}

//Timer
//-------------------------------------------------------------
void load_timer(volatile int *timer, int load_value) {
	(*(timer + 1)) = load_value;
	(*timer) = (*timer) | 0x0020;
	(*timer) = (*timer) & 0xFFDF;
}

void initialise_timer(volatile int *timer) {
	(*timer) = 0x0000;
	load_timer(timer, 0);
}

void start_timer(volatile int *timer) {
	(*timer) = (*timer) | 0x0080;
}

void stop_timer(volatile int *timer) {
	(*timer) = (*timer) & 0xFF7F;
}

int timer_value(volatile int *timer) {
	return (*(timer+2));
}


//Ethernet
//------------------------------------------------------------

//Check if there is a packet ready in the receive buffer
volatile int* mac_packet_ready() {
	int din;
	
	//Check ping buffer
	din = *(MAC_TX1 + 1535);
	if((din & 0x00000001) > 0) {
		return MAC_RX1;
	}
	
	//Check pong buffer
	din = *(MAC_TX1 + 2047);
	if((din & 0x00000001) > 0) {
		return MAC_RX2;
	}
		
	//No packets ready
	return 0;
}

//Clear the packet in the receive buffer
void mac_clear_rx_packet(volatile int *buffer) {
	if (shadow_interrupt_enable) 
		*(buffer + 511) = 0x00000008;
	else
		*(buffer + 511) = 0;
}

//Enables receive interrupts
void mac_enable_interrupts() {
	shadow_interrupt_enable = 1;
	//Enable global interrupts
	*(MAC_TX1 + 510) = 0x80000000;
	//Enables receive interrupts
	*(MAC_TX1 + 1535) = 0x00000008;
}

//Disables receive interrupts
void mac_disable_interrupts() {
	shadow_interrupt_enable = 0;
	//Disable global interrupts
	*(MAC_TX1 + 510) = 0;
	//Disable receive interrupts
	*(MAC_TX1 + 1535) = 0;
}

void mac_send_packet(int length) {
	int din;

	//Set the length
	//*((volatile int *) 0x300007F4) = length;
	*(MAC_TX1 + 509) = length;
	
	//Set the status bit
	//*((volatile int *) 0x300007FC) = 1;
	*(MAC_TX1 + 511) = 1;
	
	//Wait for it to be sent
	do {
		din = *(MAC_TX1 + 511);
		din = din & 0x00000001;
	} while(din);
}
	
int mac_tx_buffer_empty() {
	int din;

	//Check the status bit
	din = *(MAC_TX1 + 511);
	din = din & 0x00000001;
	return (din == 0);
}


//Change the MAC address
//Write the desired address into the TX buffer first
void mac_set_mac_address() {
	//Set the program and status bits
	*(MAC_TX1 + 511) = 0x00000003;
	
	//Wait for change to take place
	while(!mac_tx_buffer_empty());
}
