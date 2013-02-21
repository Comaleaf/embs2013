#ifndef __MICROBLAZE_H_
#define __MICROBLAZE_H_

#define BENCHMARK_ITERATIONS 10000

#define STATE_NUM_1     0x00
#define STATE_NUM_2     0x01
#define STATE_MESSENGER 0x02
#define STATE_COMPOSER  0x03

typedef enum { PLUS, MINUS, DIV, MULT } Operator;
typedef char State;

// states
State state_num_1(char c);
State state_num_2(char c);

// interrupt handlers
void int_handler();
void inth_mac();
void inth_switches();
void inth_buttons();
void inth_fsl();
void inth_uart();

// timers
void time_mb_div();
void time_hc_div();

void display(char* string);
void write_leds(char leds);

int main(void);

#endif