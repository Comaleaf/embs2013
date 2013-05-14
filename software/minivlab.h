#ifndef __MB_CORE_H
#define __MB_CORE_H

void initialise(void);

//Addresses
#define MAC_TX1 	((volatile int *) 0x81000000)
#define MAC_RX1 	((volatile int *) 0x81001000)
#define MAC_RX2 	((volatile int *) 0x81001800)
#define LEDS 		((volatile int *) 0x81400000)
#define SWITCHES 	((volatile int *) 0x81420000)
#define BUTTONS		((volatile int *) 0x81440000)
#define INTC 		((volatile int *) 0x81800000)
#define TIMER0 		((volatile int *) 0x83C00000)
#define UART 		((volatile int *) 0x84000000)
#define VGA			((volatile int *) 0x90000000)

//Interrupt vectors
#define INTC_UART 5
#define INTC_SWITCHES 4
#define INTC_BUTTONS 3
#define INTC_FSL 2
#define INTC_MAC 1
#define INTC_TIMER 0

//Interrupts
#define DECLARE_INTERRUPT_HANDLER(name) void (name)() __attribute__ ((interrupt_handler));
extern void mb_enable_interrupts();
extern void mb_disable_interrupts();
int intc_get_vector();
void intc_enable_all_interrupts();
void intc_disable_all_interrupts();
void intc_enable_interrupt(int num);
void intc_disable_interrupt(int num);
void intc_acknowledge_interrupt(int num);
void intc_acknowledge_all();
void intc_master_enable();

//UART
void uart_send_char(volatile int *uart, char c);
void uart_send_string(volatile int *uart, char *string);
char uart_get_char (volatile int *uart);
int uart_check_char(volatile int *uart);
void uart_enable_interrupts(volatile int *uart);
void uart_disable_interrupts(volatile int *uart);

//Basic
void set_leds(char c);
char get_switches();
void switches_enable_interrupts();
void switches_disable_interrupts();
void switches_clear_interrupt();

//Ethernet
volatile int* mac_packet_ready(void);
void mac_clear_rx_packet(volatile int *buffer);
void mac_enable_interrupts(void);
void mac_disable_interrupts(void);
void mac_send_packet(int length);
int mac_tx_buffer_empty(void);
void mac_set_mac_address(void);

//FSL macros
#define stringify(s)    tostring(s)
#define tostring(s)     #s

#define FSL_BLOCKING                           
#define FSL_NONBLOCKING                          n

#define getfslx(val, id, f1ags)      asm volatile (stringify(f1ags) "get\t%0,rfsl" stringify(id) : "=d" (val))
#define putfslx(val, id, flags)      asm volatile (stringify(flags) "put\t%0,rfsl" stringify(id) :: "d" (val))

#define fsl_isinvalid(result)               asm volatile ("addic\t%0,r0,0"  : "=d" (result))
#define fsl_iserror(error)                  asm volatile ("mfs\t%0,rmsr\n\t"  \
                                                              "andi\t%0,%0,0x10" : "=d" (error))

#endif
