#include "fsm_reset_pulse.h"

fsm_reset_pulse_state fsm_reset_pulse_current_state = s_fsm_reset_pulse_start;

void fsm_reset_pulse_start()
{
    set_pin_output();
    write_low();
    fsm_reset_pulse_current_state = s_fsm_reset_pulse_end;
    start_timer_int(480);
}

void fsm_reset_pulse_end()
{
    set_pin_input();
    fsm_reset_pulse_current_state = s_fsm_reset_pulse_start;
    start_timer_int(480);
}

void fsm_reset_pulse_execute()
{
    switch (fsm_reset_pulse_current_state)
    {
    case s_fsm_reset_pulse_start:
        fsm_reset_pulse_start();
        break;

    case s_fsm_reset_pulse_end:
        fsm_reset_pulse_end();
        break;
    }
}