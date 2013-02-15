#ifndef __UTILITIES_H_
#define __UTILITIES_H_

#define IS_DIGIT(x) (x >= 48 && x <= 57)
#define TEST_BIT(x, i)    (x & (1<<i))

// utilities
char byte2hex(char i);
void strcopy(char* src, char* dst, int length);
int pow_10(int n);
void asciify(int n, int digits, char* output);

#endif