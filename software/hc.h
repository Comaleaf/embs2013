#ifndef __HC_H_
#define __HC_H_

#define HC_OPERAND(opcode) (0x7FFFFFFF & opcode)
#define HC_OP_DIVIDE(operand) (0x00000000|(0x7FFFFFFF & operand))

int hc_divide(int n, int d);

#endif