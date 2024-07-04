#ifndef FSM_RESET_PULSE_H /* Include guard */
#define FSM_RESET_PULSE_H

#include "utils.h"

typedef enum __attribute__((packed)){
    s_fsm_reset_pulse_start,
    s_fsm_reset_pulse_end,
}fsm_reset_pulse_state;

extern fsm_reset_pulse_state fsm_reset_pulse_current_state;

void fsm_reset_pulse_execute();

#endif