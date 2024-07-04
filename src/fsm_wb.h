#ifndef FSM_WB_H /* Include guard */
#define FSM_WB_H

#include <Arduino.h>
#include "utils.h"

// typedef uint8_t fsm_wb_state;

typedef enum __attribute__((packed))
{
    s_fsm_wb_setup,
    s_fsm_wb_send_next_bit,
    s_fsm_wb_write_1_send,
    s_fsm_wb_write_1_release,
    s_fsm_wb_write_0_start,
    s_fsm_wb_write_0_end,
} fsm_wb_state;

extern fsm_wb_state fsm_wb_current_state;
extern uint8_t fsm_wb_byte, bit_mask;

void fsm_wb_execute();

#endif