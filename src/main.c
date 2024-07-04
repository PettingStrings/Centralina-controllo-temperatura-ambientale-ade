#include <Arduino.h>

#include "utils.h"
#include "fsm_main.h"

int main(void)
{
  usart_setup();
  while (1)
  {
    if (!timer_running)
    {
      fsm_main_execute();
    }
  }

  return 1;
}

ISR(TIMER2_COMPA_vect)
{
  TIMSK2 &= ~(1 << OCIE2A);
  timer_running = FALSE;
}

/*ISR(TIMER2_OVF_vect)
{
}*/