//uart.c
#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "rbuf.h"

#define UART_BAUD_SELECT(baudRate,xtalCpu) (((float)(xtalCpu))/(((float)(baudRate))*8.0)-1.0+0.5)

#ifdef UART0

#define uart0_rxcomplete (UCSR0A & (1 << RXC0))
#define uart0_txcomplete (UCSR0A & (1 << TXC0))
#define uart0_txready    (UCSR0A & (1 << UDRE0))

#ifdef UART0_IBUF
uint8_t uart0_ibuf[UART0_IBUF];
#endif //UART0_IBUF

#ifdef UART0_OBUF
uint8_t uart0_obuf[UART0_OBUF];
#endif //UART0_OBUF

#ifdef UART0_FILE
FILE _uart0io = {0};
FILE* uart0io = &_uart0io;
int uart0_putchar(char c, FILE *stream)
{
	return uart0_tx(c);
}
int uart0_getchar(FILE *stream)
{
	return uart0_rx();
}
#endif //UART0_FILE

void uart0_init(void)
{
	UCSR0A |= (1 << U2X0); // baudrate multiplier
	UBRR0L = UART_BAUD_SELECT(UART0_BAUD, F_CPU); // select baudrate
	UCSR0B = (1 << RXEN0) | (1 << TXEN0); // enable receiver and transmitter
#ifdef UART0_IBUF
	UCSR0B |= (1 << RXCIE0); // enable rx interrupt
#endif //UART0_IBUF
#ifdef UART0_OBUF
//	UCSR0B |= (1 << TXCIE0);
#endif //UART0_OBUF	

#ifdef UART0_IBUF
	rbuf_ini(uart0_ibuf, UART0_IBUF - 4);
#endif //UART0_IBUF

#ifdef UART0_OBUF
	rbuf_ini(uart0_obuf, UART0_OBUF - 4);
#endif //UART0_OBUF

#ifdef UART0_FILE
	fdev_setup_stream(uart0io, uart0_putchar, uart0_getchar, _FDEV_SETUP_WRITE | _FDEV_SETUP_READ); //setup uart0 i/o stream
#endif //UART0_FILE

}

int uart0_rx(void)
{
#ifdef UART0_IBUF
#ifdef UART0_INBL
	if (rbuf_empty(uart0_ibuf)) return -1; // for non blocking mode return -1
#else //UART0_INBL
	while (rbuf_empty(uart0_ibuf)); // wait until byte available
#endif //UART0_INBL
	return rbuf_get(uart0_ibuf);
#else //UART0_IBUF
#ifdef UART0_INBL
	if (!uart0_rxcomplete) return -1; // for non blocking mode return -1
#else //UART0_INBL
	while (!uart0_rxcomplete); // wait until byte available
#endif //UART0_INBL
	UCSR0A |= (1 << RXC0); // delete RXCflag
	return UDR0; // receive byte
#endif //UART0_IBUF
}

int uart0_tx(uint8_t c)
{
#ifdef UART0_OBUF
	if (UCSR0B & (1 << TXCIE0))
	{
#ifdef UART0_ONBL
		if (rbuf_put(uart0_obuf, c) < 0) return -1;
#else //UART0_ONBL
		while (rbuf_put(uart0_obuf, c) < 0);
#endif //UART0_ONBL
	}
	else
	{
		UCSR0B |= (1 << TXCIE0); //enable tx interrupt
		UDR0 = c; //transmit the byte
	}
#else //UART0_OBUF
#ifdef UART0_ONBL
	if (!uart0_txready) return -1; // for non blocking mode return -1
	UDR0 = c; // transmit byte
#else //UART0_ONBL
	UDR0 = c; // transmit byte
	while (!uart0_txcomplete); // wait until byte sent
	UCSR0A |= (1 << TXC0); // delete TXCflag
#endif //UART0_ONBL
#endif //UART0_OBUF
	return 0;
}

#ifdef UART0_IBUF
ISR(USART0_RX_vect)
{
	if (rbuf_put(uart0_ibuf, UDR0) < 0) // put received byte to buffer
	{
		//rx buffer full
	}
}
#endif //UART0_IBUF

#ifdef UART0_OBUF
ISR(USART0_TX_vect)
{
	int c = rbuf_get(uart0_obuf);
	if (c >= 0) 
		UDR0 = c; // transmit next byte from buffer
	else
		UCSR0B &= ~(1 << TXCIE0); // disable tx interrupt (used as tx_inprogress flag)
}
#endif //UART0_OBUF

#endif //UART0


#ifdef UART1

#define uart1_rxcomplete (UCSR1A & (1 << RXC1))
#define uart1_txcomplete (UCSR1A & (1 << TXC1))
#define uart1_txready    (UCSR1A & (1 << UDRE1))

#ifdef UART1_IBUF
uint8_t uart1_ibuf[UART1_IBUF];
#endif //UART1_IBUF

#ifdef UART1_OBUF
uint8_t uart1_obuf[UART1_OBUF];
#endif //UART1_OBUF

#ifdef UART1_FILE
FILE _uart1io = {0};
FILE* uart1io = &_uart1io;
int uart1_putchar(char c, FILE *stream)
{
	return uart1_tx(c);
}
int uart1_getchar(FILE *stream)
{
	return uart1_rx();
}
#endif //UART1_FILE

void uart1_init(void)
{
	UCSR1A |= (1 << U2X1); // baudrate multiplier
	UBRR1L = UART_BAUD_SELECT(UART1_BAUD, F_CPU); // select baudrate
	UCSR1B = (1 << RXEN1) | (1 << TXEN1); // enable receiver and transmitter
#ifdef UART1_IBUF
	UCSR1B |= (1 << RXCIE1); // enable rx interrupt
#endif //UART1_IBUF
#ifdef UART1_OBUF
//	UCSR1B |= (1 << TXCIE1);
#endif //UART1_OBUF	

#ifdef UART1_IBUF
	rbuf_ini(uart1_ibuf, UART1_IBUF - 4);
#endif //UART0_IBUF

#ifdef UART1_OBUF
	rbuf_ini(uart1_obuf, UART1_OBUF - 4);
#endif //UART1_OBUF

#ifdef UART1_FILE
	fdev_setup_stream(uart1io, uart1_putchar, uart1_getchar, _FDEV_SETUP_WRITE | _FDEV_SETUP_READ); //setup uart1 i/o stream
#endif //UART1_FILE

}


int uart1_rx(void)
{
#ifdef UART1_IBUF
#ifdef UART1_INBL
	if (rbuf_empty(uart1_ibuf)) return -1; // for non blocking mode return -1
#else //UART1_INBL
	while (rbuf_empty(uart1_ibuf)); // wait until byte available
#endif //UART1_INBL
	return rbuf_get(uart1_ibuf);
#else //UART1_IBUF
#ifdef UART1_INBL
	if (!uart1_rxcomplete) return -1; // for non blocking mode return -1
#else //UART1_INBL
	while (!uart1_rxcomplete); // wait until byte available
#endif //UART1_INBL
	UCSR1A |= (1 << RXC1); // delete RXCflag
	return UDR1; // receive byte
#endif //UART1_IBUF
}

int uart1_tx(uint8_t c)
{
#ifdef UART1_OBUF
	if (UCSR1B & (1 << TXCIE1))
	{
#ifdef UART1_ONBL
		if (rbuf_put(uart1_obuf, c) < 0) return -1;
#else //UART1_ONBL
		while (rbuf_put(uart1_obuf, c) < 0);
#endif //UART1_ONBL
	}
	else
	{
		UCSR1B |= (1 << TXCIE1); //enable tx interrupt
		UDR1 = c; //transmit the byte
	}
#else //UART1_OBUF
#ifdef UART1_ONBL
	if (!uart1_txready) return -1; // for non blocking mode return -1
	UDR1 = c; // transmit byte
#else //UART1_ONBL
	UDR1 = c; // transmit byte
	while (!uart1_txcomplete); // wait until byte sent
	UCSR1A |= (1 << TXC1); // delete TXCflag
#endif //UART1_ONBL
#endif //UART1_OBUF
	return 0;
}


#ifdef UART1_IBUF

#ifdef UART1_RFUL
const char rx_full_msg[] PROGMEM = "ERROR: rx buffer overflow\n";
void uart1_report_rx_full(void)
{
	uint8_t i;
	cli();
	for (i = 0; i < sizeof(rx_full_msg); i++)
	{
		while (!uart1_txready); // wait for ready to next char
		UDR1 = pgm_read_byte(rx_full_msg + i); //transmit the byte
		while (!uart1_txcomplete); // wait until byte sent
	}
	sei();
}
#endif //UART1_RFUL

ISR(USART1_RX_vect)
{
//	uart1_tx(UDR1);
	if (rbuf_put(uart1_ibuf, UDR1) < 0) // put received byte to buffer
	{ //rx buffer full
#ifdef UART1_RFUL //report rx full
		uart1_report_rx_full();
#endif //UART1_RFUL
		//clear buffer
		uart1_ibuf[1] = 0;
		uart1_ibuf[2] = 0;
	}
}
#endif //UART1_IBUF

#ifdef UART1_OBUF
ISR(USART1_TX_vect)
{
	int c = rbuf_get(uart1_obuf);
	if (c >= 0) 
		UDR1 = c; // transmit next byte from buffer
	else
		UCSR1B &= ~(1 << TXCIE1); // disable tx interrupt (used as tx_inprogress flag)
}
#endif //UART1_OBUF


#endif //UART1
