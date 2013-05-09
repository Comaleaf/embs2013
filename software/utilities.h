#ifndef __UTILITIES_H_
#define __UTILITIES_H_

#define IS_DIGIT(x) (x >= '0' && x <= '9')
#define IS_HEXIT(x) ((x >= '0' && x <= '9') || (x >= 'A' && x <= 'F') || (x >= 'a' && x <= 'f'))
#define TEST_BIT(x, i) ((x) & (1<<(i)))

// utilities
char byte2hex(char i);
char* strncpy(char* dst, char* src, int length);
int pow_10(int n);
char* int_to_digits(int n, int digits, char* output);
char* int2digit(int n, char* output);

#endif