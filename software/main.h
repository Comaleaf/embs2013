#ifndef __MAIN_H_
#define __MAIN_H_

char should_reset;
char active_channel;

// Interrupt handlers
void int_handler();
void inth_mac();
void inth_switches();
void inth_uart();

int main(void);

#endif