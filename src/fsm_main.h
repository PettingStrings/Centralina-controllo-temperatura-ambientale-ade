#ifndef FSM_MAIN_H /* Include guard */
#define FSM_MAIN_H

#include <Arduino.h>

#include "utils.h"
#include "fsm_wb.h"
#include "fsm_reset_pulse.h"

typedef enum __attribute__((packed))
{
    s_fsm_main_send_reset_pulse_0,

    s_fsm_main_send_rom_skip,
    s_fsm_main_setup_wb_rom_skip,

    s_fsm_main_setup_wb_func_convert,
    s_fsm_main_send_func_convert,

    s_fsm_main_send_reset_1,

    s_fsm_main_setup_wb_rom_match_1,
    s_fsm_main_send_rom_match_1,

    s_fsm_main_send_serial_id_1,
    s_fsm_main_send_func_read_1,
    s_fsm_main_read_temp_1,

    s_fsm_main_send_reset_2,

    s_fsm_main_setup_wb_rom_match_2,
    s_fsm_main_send_rom_match_2,

    s_fsm_main_send_serial_id_2,
    s_fsm_main_send_func_read_2,
    s_fsm_main_read_temp_2,

    s_fsm_main_send_reset_3,

    s_fsm_main_setup_wb_rom_match_3,
    s_fsm_main_send_rom_match_3,

    s_fsm_main_send_serial_id_3,
    s_fsm_main_send_func_read_3,
    s_fsm_main_read_temp_3,

    s_fsm_main_send_reset_4,

    s_fsm_main_setup_wb_rom_match_4,
    s_fsm_main_send_rom_match_4,

    s_fsm_main_send_serial_id_4,
    s_fsm_main_send_func_read_4,
    s_fsm_main_read_temp_4,

    s_fsm_main_process_temps,

s_fsm_start_transmission,
s_fsm_check_temp_1,
s_fsm_check_temp_2,
s_fsm_check_temp_3,
s_fsm_check_temp_4,
s_fsm_end_transmission,

    s_fsm_wait_pause
} fsm_main_state;

extern fsm_main_state fsm_main_current_state;

typedef enum __attribute__((packed))
{
    ROM_SKIP = 0xCC,
    ROM_MATCH = 0x55,
    FUNC_CONVERT = 0x44,
    FUNC_READ = 0xBE
} rom_codes;

void fsm_main_execute();

#endif