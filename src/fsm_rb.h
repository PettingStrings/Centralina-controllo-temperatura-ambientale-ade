#ifndef FSM_RB_H /* Include guard */
#define FSM_RB_H

#include <Arduino.h>

typedef enum __attribute__((packed))
{
   s_fsm_rb_start,
   s_fsm_rb_signal_read,
   s_fsm_rb_listen,
} fsm_rb_state;

extern fsm_rb_state fsm_rb_current_state;
extern uint8_t read_byte;

void fsm_rb_execute();

#endif