#ifndef __MAIN_H_
#define __MAIN_H_

// Set channels
int get_channels();
void set_channels(int new_channels);

// Interrupt handlers
void int_handler();
void inth_mac();
void inth_switches();
void inth_uart();

int main(void);

#endif