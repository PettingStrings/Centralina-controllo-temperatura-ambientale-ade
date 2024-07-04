#include "fsm_rb.h"
#include "utils.h"

fsm_rb_state fsm_rb_current_state = s_fsm_rb_start;
uint8_t bitmask = 0x01;
uint8_t read_byte = 0x00;

void fsm_rb_start();
void fsm_rb_signal_read();
void fsm_rb_listen();

void fsm_rb_start()
{
    bitmask = 0x01;
    read_byte = 0x00;
    fsm_rb_current_state = s_fsm_rb_signal_read;
    fsm_rb_signal_read();
}

void fsm_rb_signal_read()
{
    set_pin_output();
    write_low();
    fsm_rb_current_state = s_fsm_rb_listen;
    //start_timer_int(1);
}

void fsm_rb_listen()
{
    set_pin_input();
    uint8_t bit = read_pin();
    if (bit)
        read_byte |= bitmask;

    bitmask <<= 1;
    fsm_rb_current_state = bitmask ? s_fsm_rb_signal_read : s_fsm_rb_start;
    start_timer_int(60);
}

void fsm_rb_execute()
{
    switch (fsm_rb_current_state)
    {
    case s_fsm_rb_start:
        fsm_rb_start();
        break;
    case s_fsm_rb_signal_read:
        fsm_rb_signal_read();
        break;
    case s_fsm_rb_listen:
        fsm_rb_listen();
        break;
    }
}