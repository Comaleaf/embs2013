#ifndef __MAIN_H_
#define __MAIN_H_

#define WIDTH_8BIT  0
#define WIDTH_16BIT 1

#define RATE_8K     0
#define RATE_11K    1
#define RATE_22K    2
#define RATE_44K    3

typedef struct {
	char width;
	char rate;
	char interval;
} ChanInfo;

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