#ifndef __MAIN_H_
#define __MAIN_H_

extern struct {
	char channel = 0;
	char width   = 0;
	char rate    = 0;
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