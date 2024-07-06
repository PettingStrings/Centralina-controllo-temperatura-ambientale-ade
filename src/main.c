#include <Arduino.h>

#include "utils.h"
#include "fsm_main.h"

volatile uint8_t timerDone = FALSE;

int main(void)
{
  DDRD |= (1 << PIND3);

  DDRB |= (1 << DDB5); // Pin 13

  // start_alarm();
  //start_timer_1();

  while (1)
  {
    if (!timer_2_running)
    {
      fsm_main_execute();
    }

    if(timer_1_compa){
      timer_1_compa = FALSE;
      reset_timer_1();
      ticks_passed++;
    }
  }

  return 1;
}

ISR(TIMER2_COMPA_vect)
{
  TIMSK2 &= ~(1 << OCIE2A);
  TCNT2 = 0;
  TIFR2 |= (1 << OCF2A);
  timer_2_running = FALSE;
}

ISR(TIMER0_COMPA_vect)
{
  PORTD ^= (1 << PIND3);
}

ISR(TIMER1_COMPA_vect)
{
  timer_1_compa = TRUE;
}
