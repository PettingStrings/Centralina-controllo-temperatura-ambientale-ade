#ifndef UTILS_DALLAS_TEMP_H
#define UTILS_DALLAS_TEMP_H

#include <avr/io.h>

#define TRUE 1
#define FALSE 0
#define RECOVERY 5

#define BAUD 9600
#define BRC F_CPU/16/BAUD-1

#define PIN_ID PIND2
#define PIN_REGISTER DDRD
#define PIN_INPUT PIND
#define PIN_OUTPUT PORTD

extern volatile uint8_t timer_running;

void start_timer_int(int micro_time);

void usart_setup();
void UART_write_str(char *str);
void UART_write_int(int val);
void UART_write_byte(uint8_t data);

void set_pin_output();
void set_pin_input();
uint8_t read_pin();
void write_low();
void write_high();

#endif