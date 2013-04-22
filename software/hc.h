#ifndef __HC_H_
#define __HC_H_

#define HC_MODE  0
#define HC_8BIT  1
#define HC_16BIT 2

#define HC_PUT(opcode, operand) putfslx((opcode<<30U)|(0x3FFFFFFF&operand), 0, FSL_BLOCKING)

void hc_put_char(unsigned char c);
void hc_put_short(short s);
void hc_set_mode(char rate, char width);

#endif