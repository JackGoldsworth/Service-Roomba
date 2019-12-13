#include "uart.h"

void uart_init(void) {
	// Enable UART1
	SYSCTL_RCGCUART_R |= 0b00000010;
	// Enable Port B
	SYSCTL_RCGCGPIO_R |= 0b00000010;
	// Set bits 0 and 1 to use alternative functions.
	GPIO_PORTB_AFSEL_R |= 0b0011;

	GPIO_PORTB_PCTL_R &= 0b00000000;
	GPIO_PORTB_PCTL_R |= 0b00010001;
	// Set PB0 to recieve and PB1 to transmit
	GPIO_PORTB_DEN_R |= 0b00000011;
	GPIO_PORTB_DIR_R &= 0b11111110;
	GPIO_PORTB_DIR_R |= 0b00000010;


	UART1_CTL_R &= 0b11111110;
	// Sets the UART to use the WiFi rate (115200 baud).
	UART1_IBRD_R = 8;
	UART1_FBRD_R = 44;
	UART1_LCRH_R |= UART_LCRH_WLEN_8;
	UART1_CC_R &= 0xFFFFFFF0;
	UART1_CTL_R |= 0b1100000001;
}

void uart_sendChar(char data) {
	while (UART1_FR_R & 0x20) {}
	UART1_DR_R = data;
}

int uart_receive(void) {
	int i;
	int value;
	while (UART1_FR_R & 0x10) {}
	i = UART1_DR_R;
	if (i & 0xF00) {
		GPIO_PORTE_DATA_R = 0xF;
	}
	else {
		value = (int)UART1_DR_R & 0xFF;
	}
	return value;
}

void uart_sendStr(const char* data) {
	while (*data != '\0') {
		uart_sendChar(*data);
		data++;
		if (*data == '\r') {
			uart_sendChar('\n');
		}
	}
}
