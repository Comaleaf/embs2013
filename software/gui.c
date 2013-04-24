#include "vlab.h"
#include "utilities.h"
#include "gui.h"

#define IS_PLUS_OR_MINUS(x) (x == '+' || x == '-')

char input[50];
short cursor;

void prompt() {
	uart_send_string(UART, "\r\n> ");
	
	// wipe input
	while (cursor > 0) {
		input[cursor--] = 0;
	}
}

void accept(char c) {
	if (c == '\r') {
		prompt();
	}
	else if (IS_DIGIT(c) && (IS_DIGIT(input[cursor-1]) || IS_PLUS_OR_MINUS(input[cursor-1]))) {
		input[cursor++] = c;
		uart_send_char(UART, c);
	}
	else if (IS_PLUS_OR_MINUS(c) && (cursor == 0 || input[cursor-1] == ' ')) {
		input[cursor++] = c;
		uart_send_char(UART, c);
	}
	else if (c == ' ' && IS_DIGIT(input[cursor-1]) )  {
		input[cursor++] = c;
		uart_send_char(UART, c);
	}
}

void process() {

}