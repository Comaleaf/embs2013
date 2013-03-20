#ifndef __HC_H_
#define __HC_H_

#define HC_8BIT  1
#define HC_16BIT 2
#define HC_CLEAR 4

#define HC_OPERAND(opcode) (0x7FFFFFFF & opcode)
#define HC_OP_SEND_CHAR(c) (0x10000000|(0x000000FF & c))
#define HC_OP_SEND_SHORT(s) (0x20000000|(0x0000FFFF & s))

void hc_send_char(unsigned char c);
void hc_send_short(short s);
void hc_clear_buffer();

#endif