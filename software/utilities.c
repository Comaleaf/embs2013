#include "utilities.h"

char byte2hex(char i) {
	if (i < 10) return i + 48;
	else        return (i-10) + 65;
}

void strcopy(char* src, char* dst, int length) {
	while (length-- > 0) {
		src[length] = dst[length];
	}
}

int pow_10(int n) {
	if (n <= 0) return 1;
	else        return 10 * pow_10(n-1);
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