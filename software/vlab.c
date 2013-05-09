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
