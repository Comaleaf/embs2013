#ifndef __MAIN_H_
#define __MAIN_H_

typedef enum { PLUS, MINUS, MULT, DIV } Operator;
typedef enum { NUM_1, NUM_2, MESSENGER, COMPOSER_1, COMPOSER_2 } State;

typedef struct {
	char dest;
	char source;
	char data[1000];
	int length;
} Message;

// States
State state_num_1(unsigned char c);
State state_num_2(unsigned char c);
State state_messenger(unsigned char c);
State state_composer_1(unsigned char c);
State state_composer_2(unsigned char c);

// Interrupt handlers
void int_handler();
void inth_mac();
void inth_switches();
void inth_buttons();
void inth_fsl();
void inth_uart();

// Outputs
void display(char* string);
void display_char(char c);
void write_leds(char leds);

int main(void);

#endif