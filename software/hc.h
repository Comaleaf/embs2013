#ifndef __HC_H_
#define __HC_H_

#include "microblaze_0.h"

#define HC_STATE_ACCEPT 0x0
#define HC_STATE_PRINT  0x1
#define HC_STATE_IN_N   0x2
#define HC_STATE_IN_D   0x3

void hc_setstate(State state);
void hc_print(char *output);

#endif