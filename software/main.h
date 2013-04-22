#ifndef __MAIN_H_
#define __MAIN_H_

struct {
	char channel;
	char width;
	char rate;
} state;

// Interrupt handlers
void int_handler();
void inth_mac();
void inth_switches();
void inth_uart();

// Outputs
void display(char* string);
void display_char(char c);
void write_leds(char leds);

int main(void);

#endif