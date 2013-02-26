#include "utilities.h"

char byte2hex(char i) {
	if (i < 10) return i + 48;
	else        return (i-10) + 65;
}

char* strncpy(char* dst, char* src, int length) {
	while (length-- > 0) {
		dst[length] = src[length];
	}
	return dst;
}

int pow_10(int n) {
	int v = 1;
	
	while (n-- > 1) {
		v *= 10;
	}
	
	return v;
}

void asciify(int n, int digits, char* output) {
	char seen = 0;
	int ord;
	output[digits] = 0;
	
	for (int i = 0; i <= digits; i++) {
		ord = pow_10(digits-i);
		
		if (ord <= n) {
			seen = 1;
			output[i] = 48 + n/ord;
			n -= (n/ord) * ord;
		}
		else if (seen || i == digits) {
			output[i] = '0';
		}
		else {
			output[i] = ' ';
		}
	}
}