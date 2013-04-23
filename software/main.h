#ifndef __MAIN_H_
#define __MAIN_H_

char has_switched;
char active_channel;

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