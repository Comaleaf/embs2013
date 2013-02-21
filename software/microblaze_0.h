#ifndef __MICROBLAZE_H_
#define __MICROBLAZE_H_

#define STATE_NUM_1     0x00
#define STATE_NUM_2     0x01
#define STATE_MESSENGER 0x02
#define STATE_COMPOSER  0x03

typedef enum { PLUS, MINUS, DIV, MULT } Operator;
typedef enum { NUM_1, NUM_2, MESSENGER, COMPOSER } State;

// States
State state_num_1(char c);
State state_num_2(char c);
State state_messenger(char c);
State state_composer(char c);

// Interrupt handlers
void int_handler();
void inth_mac();
void inth_switches();
void inth_buttons();
void inth_fsl();
void inth_uart();

// Outputs
void display(char* string);
void write_leds(char leds);

int main(void);

#endif