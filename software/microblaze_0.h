#ifndef __MICROBLAZE_H_
#define __MICROBLAZE_H_

typedef enum { PLUS, MINUS, DIV, MULT } Operator;
typedef enum { NUM_1, NUM_2, MESSENGER, COMPOSER_1, COMPOSER_2 } State;

// States
State state_num_1(char c);
State state_num_2(char c);
State state_messenger(char c);
State state_composer_1(char c);
State state_composer_2(char c);

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