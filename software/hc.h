#ifndef __HC_H_
#define __HC_H_

#define HC_8BIT  1
#define HC_16BIT 2
#define HC_CLEAR 4

#define HC_PUT(opcode)          putfslx(opcode, 0, FSL_BLOCKING)
#define HC_PUT(opcode, operand) putfslx((opcode<<7)|(0x7FFFFFFF&operand), 0, FSL_BLOCKING)

void hc_send_char(unsigned char c);
void hc_send_short(short s);
void hc_clear_buffer();

#endif