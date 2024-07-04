#include "fsm_wb.h"

fsm_wb_state fsm_wb_current_state = s_fsm_wb_setup;

uint8_t fsm_wb_byte = 0, 
bit_mask = 0x01;

void fsm_wb_write_1_send()
{
    set_pin_output();
    write_low();
    fsm_wb_current_state = s_fsm_wb_write_1_release;
}

void fsm_wb_write_1_release()
{
    set_pin_input();
    start_timer_int(50);
    fsm_wb_current_state = bit_mask ? s_fsm_wb_send_next_bit : s_fsm_wb_setup;
}

void fsm_wb_write_0_start()
{
    set_pin_output();
    write_low();
    fsm_wb_current_state = s_fsm_wb_write_0_end;
    start_timer_int(60);
}

void fsm_wb_write_0_end()
{
    set_pin_input();
    fsm_wb_current_state = bit_mask ? s_fsm_wb_send_next_bit : s_fsm_wb_setup;
    // start_timer_int(5);
}

void fsm_wb_send_next_bit()
{
    if (fsm_wb_byte & bit_mask)
        fsm_wb_write_1_send();
    else
        fsm_wb_write_0_start();
    bit_mask <<= 1;
}

void fsm_wb_setup()
{
    bit_mask = 0x01;
    fsm_wb_current_state = s_fsm_wb_send_next_bit;
    fsm_wb_send_next_bit();
}

void fsm_wb_execute()
{
    switch (fsm_wb_current_state)
    {
    case s_fsm_wb_setup:
        fsm_wb_setup();
        break;
    case s_fsm_wb_send_next_bit:
        fsm_wb_send_next_bit();
        break;
    case s_fsm_wb_write_1_send:
        fsm_wb_write_1_send();
        break;
    case s_fsm_wb_write_1_release:
        fsm_wb_write_1_release();
        break;
    case s_fsm_wb_write_0_start:
        fsm_wb_write_0_start();
        break;
    case s_fsm_wb_write_0_end:
        fsm_wb_write_0_end();
        break;
    }
}
