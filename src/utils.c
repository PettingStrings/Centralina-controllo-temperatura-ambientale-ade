#include "utils.h"
#include <Arduino.h>

volatile uint8_t timer_running = FALSE;

void usart_setup(){
    // USART
  UBRR0H = ((BRC) >> 8);
  UBRR0L = BRC;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_write_byte(uint8_t data) {
  /* Wait for empty transmit buffer */
  while (!(UCSR0A & (1 << UDRE0)));
  UDR0 = data;
}

void UART_write_int(int val) {
  do {
    UART_write_byte('0' + val % 10);
    val = val / 10;
  } while (val > 0);
  UART_write_byte('\0');
}

void UART_write_str(char* str) {
  int i;
  for (i = 1; (*str)!='\0'; i++ ) {
    UART_write_byte(*str);
    str++;
  }
  UART_write_byte('\r');
  UART_write_byte('\n');
}

unsigned char USART_receive() {

  while (!(UCSR0A & (1 << RXC0)));
  return UDR0;

}

void start_timer_int(int micro_time)
{
  // Prescaler options: 1, 8, 32, 64, 128, 256, 1024
  // Formula: compare match value = (desired interval * clock frequency) / prescaler - 1
  cli();
  TCCR2A = 0; // Clear Timer2 control register A
  TCCR2B = 0; // Clear Timer2 control register B
  TCNT2 = 0;  // Initialize Timer2 counter to 0
  uint8_t prescaler = 8;
  if (micro_time > 128)
    prescaler = 32;
  TCCR2A |= (1 << WGM21); // CTC mode
  TCCR2B |= (1 << CS21);  // Prescaler 8
  if (prescaler == 32)
    TCCR2B |= (1 << CS20); // Prescaler 32

  OCR2A = micro_time * 16 / prescaler - 1; // Set compare match value
  // Enable Timer2 compare match interrupt
  TIMSK2 |= (1 << OCIE2A);
  timer_running = TRUE;
  sei();
}

void set_pin_output()
{
    PIN_REGISTER |= (1 << PIN_ID);
}

void set_pin_input()
{
    PIN_REGISTER &= ~(1 << PIN_ID);
}

uint8_t read_pin()
{
    return PIN_INPUT & _BV(PIN_ID);
}

void write_low()
{
    PIN_OUTPUT &= ~(1 << PIN_ID);
}

void write_high()
{
    PIN_OUTPUT |= (1 << PIN_ID);
}
