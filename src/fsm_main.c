#include "fsm_main.h"
#include "fsm_wb.h"
#include "fsm_reset_pulse.h"
#include "fsm_rb.h"

// const uint8_t DALLAS_SERIAL_1[] = {0xba, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10};
// 1000000000000fba - 0001000000000000000000000000000000000000000000000000111110111010
const uint8_t DALLAS_SERIAL_1[] = {0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xba};
const uint8_t DALLAS_SERIAL_2[] = {0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x8f};
const uint8_t DALLAS_SERIAL_3[] = {0x10, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0xe3};
const uint8_t DALLAS_SERIAL_4[] = {0x10, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x62};

char buf[50];

int8_t temp_1, temp_2, temp_3, temp_4;
int8_t temp_minima = 18, temp_massima = 25,
       temp_media, temp_tolleranza = 5; // Valori in gradi

uint8_t temp_msb = 0xff, temp_lsb = 0xff;
uint8_t serial_byte_counter = 0;
uint8_t lsb_read = 0;

fsm_main_state fsm_main_current_state = s_fsm_main_send_reset_pulse_0;

void fsm_main_send_reset_pulse_0();

void fsm_main_send_rom_skip();
void fsm_main_setup_wb_rom_skip();

void fsm_main_send_func_convert();
void fsm_main_setup_wb_func_convert();

void fsm_main_send_reset_1();

void fsm_main_send_rom_match_1();
void fsm_main_setup_wb_rom_match_1();

void fsm_main_send_serial_id_1();
void fsm_main_send_func_read_1();
void fsm_main_read_temp_1();

void fsm_main_send_reset_2();

void fsm_main_setup_wb_rom_match_2();
void fsm_main_send_rom_match_2();

void fsm_main_send_serial_id_2();
void fsm_main_send_func_read_2();
void fsm_main_read_temp_2();

void fsm_main_send_reset_3();

void fsm_main_setup_wb_rom_match_3();
void fsm_main_send_rom_match_3();

void fsm_main_send_serial_id_3();
void fsm_main_send_func_read_3();
void fsm_main_read_temp_3();

void fsm_main_send_reset_4();

void fsm_main_setup_wb_rom_match_4();
void fsm_main_send_rom_match_4();

void fsm_main_send_serial_id_4();
void fsm_main_send_func_read_4();
void fsm_main_read_temp_4();

void fsm_main_process_temps();

void fsm_start_transmission();
void fsm_check_temp_1();
void fsm_check_temp_2();
void fsm_check_temp_3();
void fsm_check_temp_4();
void fsm_end_transmission();

void fsm_main_ended();

uint8_t generic_reset_pulse()
{
    fsm_reset_pulse_execute();
    return (fsm_reset_pulse_current_state == s_fsm_reset_pulse_start);
}

uint8_t generic_send_byte()
{
    fsm_wb_execute();
    return (fsm_wb_current_state == s_fsm_wb_setup);
}

void fsm_main_send_reset_pulse_0()
{
    if (generic_reset_pulse())
        fsm_main_current_state = s_fsm_main_setup_wb_rom_skip;
}

void fsm_main_send_rom_skip()
{
    if (generic_send_byte())
        fsm_main_current_state = s_fsm_main_setup_wb_func_convert;
}

void fsm_main_setup_wb_rom_skip()
{
    fsm_wb_byte = ROM_SKIP;
    fsm_main_current_state = s_fsm_main_send_rom_skip;
    fsm_main_send_rom_skip();
}

void fsm_main_setup_wb_func_convert()
{
    fsm_wb_byte = FUNC_CONVERT;
    fsm_wb_current_state = s_fsm_wb_setup;
    fsm_main_current_state = s_fsm_main_send_func_convert;
    fsm_main_send_func_convert();
}

void fsm_main_send_func_convert()
{
    if (generic_send_byte())
        fsm_main_current_state = s_fsm_main_send_reset_1;
}

void fsm_main_send_reset_1()
{
    if (generic_reset_pulse())
        fsm_main_current_state = s_fsm_main_setup_wb_rom_match_1;
}

void fsm_main_setup_wb_rom_match_1()
{
    fsm_wb_byte = ROM_MATCH;
    fsm_main_current_state = s_fsm_main_send_rom_match_1;
    fsm_main_send_rom_match_1();
}

void fsm_main_send_rom_match_1()
{
    if (generic_send_byte())
    {
        fsm_main_current_state = s_fsm_main_send_serial_id_1;
    }
}

void fsm_main_send_serial_id_1()
{
    fsm_wb_byte = *(DALLAS_SERIAL_1 + serial_byte_counter);
    fsm_wb_execute();
    if (fsm_wb_current_state == s_fsm_wb_setup)
    {
        if (serial_byte_counter == 7)
        {
            serial_byte_counter = 0;

            fsm_main_current_state = s_fsm_main_send_func_read_1;
            fsm_wb_byte = FUNC_READ;
        }
        else
        {
            serial_byte_counter++;
        }
    }
}

void fsm_main_send_func_read_1()
{
    if (generic_send_byte())
        fsm_main_current_state = s_fsm_main_read_temp_1;
}

void fsm_main_read_temp_1()
{
    fsm_rb_execute();
    if (fsm_rb_current_state == s_fsm_rb_start)
    {
        if (lsb_read)
        {
            temp_msb = read_byte;
            temp_1 = ((temp_msb << 8) | (temp_lsb)) >> 1;
            lsb_read = 0;
            fsm_main_current_state = s_fsm_main_send_reset_2;
        }
        else
        {
            temp_lsb = read_byte;
            lsb_read = 1;
        }
    }
}
// check
void fsm_main_send_reset_2()
{
    if (generic_reset_pulse())
        fsm_main_current_state = s_fsm_main_setup_wb_rom_match_2;
}

void fsm_main_setup_wb_rom_match_2()
{
    fsm_wb_byte = ROM_MATCH;
    fsm_main_current_state = s_fsm_main_send_rom_match_2;
    fsm_main_send_rom_match_2();
}

void fsm_main_send_rom_match_2()
{
    if (generic_send_byte())
    {
        fsm_main_current_state = s_fsm_main_send_serial_id_2;
    }
}

void fsm_main_send_serial_id_2()
{
    fsm_wb_byte = *(DALLAS_SERIAL_2 + serial_byte_counter);
    fsm_wb_execute();
    if (fsm_wb_current_state == s_fsm_wb_setup)
    {
        if (serial_byte_counter == 7)
        {
            serial_byte_counter = 0;

            fsm_main_current_state = s_fsm_main_send_func_read_2;
            fsm_wb_byte = FUNC_READ;
        }
        else
        {
            serial_byte_counter++;
        }
    }
}

void fsm_main_send_func_read_2()
{
    if (generic_send_byte())
        fsm_main_current_state = s_fsm_main_read_temp_2;
}

void fsm_main_read_temp_2()
{
    fsm_rb_execute();
    if (fsm_rb_current_state == s_fsm_rb_start)
    {
        if (lsb_read)
        {
            temp_msb = read_byte;
            temp_2 = ((temp_msb << 8) | (temp_lsb)) >> 1;
            lsb_read = 0;
            fsm_main_current_state = s_fsm_main_send_reset_3;
        }
        else
        {
            temp_lsb = read_byte;
            lsb_read = 1;
        }
    }
}

void fsm_main_send_reset_3()
{
    if (generic_reset_pulse())
        fsm_main_current_state = s_fsm_main_setup_wb_rom_match_3;
}

void fsm_main_setup_wb_rom_match_3()
{
    fsm_wb_byte = ROM_MATCH;
    fsm_main_current_state = s_fsm_main_send_rom_match_3;
    fsm_main_send_rom_match_3();
}

void fsm_main_send_rom_match_3()
{
    if (generic_send_byte())
    {
        fsm_main_current_state = s_fsm_main_send_serial_id_3;
    }
}

void fsm_main_send_serial_id_3()
{
    fsm_wb_byte = *(DALLAS_SERIAL_3 + serial_byte_counter);
    fsm_wb_execute();
    if (fsm_wb_current_state == s_fsm_wb_setup)
    {
        if (serial_byte_counter == 7)
        {
            serial_byte_counter = 0;

            fsm_main_current_state = s_fsm_main_send_func_read_3;
            fsm_wb_byte = FUNC_READ;
        }
        else
        {
            serial_byte_counter++;
        }
    }
}

void fsm_main_send_func_read_3()
{
    if (generic_send_byte())
        fsm_main_current_state = s_fsm_main_read_temp_3;
}

void fsm_main_read_temp_3()
{
    fsm_rb_execute();
    if (fsm_rb_current_state == s_fsm_rb_start)
    {
        if (lsb_read)
        {
            temp_msb = read_byte;
            temp_3 = ((temp_msb << 8) | (temp_lsb)) >> 1;
            lsb_read = 0;
            fsm_main_current_state = s_fsm_main_send_reset_4;
        }
        else
        {
            temp_lsb = read_byte;
            lsb_read = 1;
        }
    }
}

void fsm_main_send_reset_4()
{
    if (generic_reset_pulse())
        fsm_main_current_state = s_fsm_main_setup_wb_rom_match_4;
}

void fsm_main_setup_wb_rom_match_4()
{
    fsm_wb_byte = ROM_MATCH;
    fsm_main_current_state = s_fsm_main_send_rom_match_4;
    fsm_main_send_rom_match_4();
}

void fsm_main_send_rom_match_4()
{
    if (generic_send_byte())
    {
        fsm_main_current_state = s_fsm_main_send_serial_id_4;
    }
}

void fsm_main_send_serial_id_4()
{
    fsm_wb_byte = *(DALLAS_SERIAL_4 + serial_byte_counter);
    fsm_wb_execute();
    if (fsm_wb_current_state == s_fsm_wb_setup)
    {
        if (serial_byte_counter == 7)
        {
            serial_byte_counter = 0;

            fsm_main_current_state = s_fsm_main_send_func_read_4;
            fsm_wb_byte = FUNC_READ;
        }
        else
        {
            serial_byte_counter++;
        }
    }
}

void fsm_main_send_func_read_4()
{
    if (generic_send_byte())
        fsm_main_current_state = s_fsm_main_read_temp_4;
}

void fsm_main_read_temp_4()
{
    fsm_rb_execute();
    if (fsm_rb_current_state == s_fsm_rb_start)
    {
        if (lsb_read)
        {
            temp_msb = read_byte;
            temp_4 = (((temp_msb << 8)) | ((temp_lsb))) >> 1;
            lsb_read = 0;
            fsm_main_current_state = s_fsm_main_process_temps;
        }
        else
        {
            temp_lsb = read_byte;
            lsb_read = 1;
        }
    }
}

void fsm_main_process_temps()
{
    if (generic_reset_pulse())
    {
        temp_media = (temp_1 + temp_2 + temp_3 + temp_4) / 4;

        if (temp_media < temp_minima)
        {                           // Raffreddamento
            PORTD &= ~(1 << PIND5); // Spegni Riscaldamento
            DDRD |= (1 << PIND4);
            PORTD |= (1 << PIND4);
        }
        else if (temp_media > temp_massima)
        {                           // Riscaldamento
            PORTD &= ~(1 << PIND4); // Spegni Raffreddamento
            DDRD |= (1 << PIND5);
            PORTD |= (1 << PIND5);
        }
        else
        {
            PORTD &= ~(1 << PIND5);
            PORTD &= ~(1 << PIND4);
        }

        if (temp_1 < temp_media - temp_tolleranza || temp_1 > temp_media + temp_tolleranza ||
            temp_2 < temp_media - temp_tolleranza || temp_2 > temp_media + temp_tolleranza ||
            temp_3 < temp_media - temp_tolleranza || temp_3 > temp_media + temp_tolleranza ||
            temp_4 < temp_media - temp_tolleranza || temp_4 > temp_media + temp_tolleranza)
            fsm_main_current_state = s_fsm_start_transmission;
        else
            fsm_main_current_state = s_fsm_wait_pause;
    }
}
uint8_t pause = FALSE;
void fsm_start_transmission()
{
    if (!timer_1_running && !pause)
    {
        start_alarm();
        start_timer_1();
    }

    if (ticks_passed >= 2 * 2)
    {
        stop_alarm();
        if (ticks_passed >= 3 * 2)
        {
            pause = FALSE;
            stop_timer_1();
            fsm_main_current_state = s_fsm_check_temp_1;
        }
    }
}
void fsm_check_temp_1()
{
    if (!(temp_1 < temp_media - temp_tolleranza || temp_1 > temp_media + temp_tolleranza))
    {
        fsm_main_current_state = s_fsm_check_temp_2;
    }

    if (!timer_1_running && !pause)
    {
        start_alarm();
        start_timer_1();
    }

    if (ticks_passed >= 1 * 2)
    {
        stop_alarm();
        if (ticks_passed >= 2 * 2)
        {
            pause = FALSE;
            stop_timer_1();
            fsm_main_current_state = s_fsm_check_temp_2;
        }
    }
}
void fsm_check_temp_2()
{
    if (!(temp_2 < temp_media - temp_tolleranza || temp_2 > temp_media + temp_tolleranza))
    {
        fsm_main_current_state = s_fsm_check_temp_3;
    }

    if (!timer_1_running && !pause)
    {
        start_alarm();
        start_timer_1();
    }

    if (ticks_passed >= 2 * 2)
    {
        stop_alarm();
        if (ticks_passed >= 3 * 2)
        {
            pause = FALSE;
            stop_timer_1();
            fsm_main_current_state = s_fsm_check_temp_3;
        }
    }
}
void fsm_check_temp_3()
{
    if (!(temp_3 < temp_media - temp_tolleranza || temp_3 > temp_media + temp_tolleranza))
    {
        fsm_main_current_state = s_fsm_check_temp_4;
    }

    if (!timer_1_running && !pause)
    {
        start_alarm();
        start_timer_1();
    }

    if (ticks_passed >= 3 * 2)
    {
        stop_alarm();
        if (ticks_passed >= 4 * 2)
        {
            pause = FALSE;
            stop_timer_1();
            fsm_main_current_state = s_fsm_check_temp_4;
        }
    }
}
void fsm_check_temp_4()
{
    if (!(temp_1 < temp_media - temp_tolleranza || temp_1 > temp_media + temp_tolleranza))
    {
        fsm_main_current_state = s_fsm_end_transmission;
    }
    if (!timer_1_running && !pause)
    {
        start_alarm();
        start_timer_1();
    }

    if (ticks_passed >= 4 * 2)
    {
        stop_alarm();
        if (ticks_passed >= 5 * 2)
        {
            pause = FALSE;
            stop_timer_1();
            fsm_main_current_state = s_fsm_end_transmission;
        }
    }
}

void fsm_end_transmission()
{
    if (!timer_1_running && !pause)
    {
        start_alarm();
        start_timer_1();
    }

    if (ticks_passed >= 2 * 2)
    {
        stop_alarm();
        if (ticks_passed >= 3 * 2)
        {
            pause = FALSE;
            stop_timer_1();
            fsm_main_current_state = s_fsm_wait_pause;
        }
    }
}

void fsm_wait_pause()
{
    if (!timer_1_running)
    {
        start_timer_1();
    }
    if (ticks_passed >= 2 * 2)
    {
        stop_timer_1();
        fsm_main_current_state = s_fsm_main_send_reset_pulse_0;
    }
}

void fsm_main_execute()
{
    switch (fsm_main_current_state)
    {
    case s_fsm_main_send_reset_pulse_0:
        fsm_main_send_reset_pulse_0();
        break;
    case s_fsm_main_setup_wb_rom_skip:
        fsm_main_setup_wb_rom_skip();
        break;
    case s_fsm_main_send_rom_skip:
        fsm_main_send_rom_skip();
        break;
    case s_fsm_main_setup_wb_func_convert:
        fsm_main_setup_wb_func_convert();
        break;
    case s_fsm_main_send_func_convert:
        fsm_main_send_func_convert();
        break;
    case s_fsm_main_send_reset_1:
        fsm_main_send_reset_1();
        break;
    case s_fsm_main_setup_wb_rom_match_1:
        fsm_main_setup_wb_rom_match_1();
        break;
    case s_fsm_main_send_rom_match_1:
        fsm_main_send_rom_match_1();
        break;
    case s_fsm_main_send_serial_id_1:
        fsm_main_send_serial_id_1();
        break;
    case s_fsm_main_send_func_read_1:
        fsm_main_send_func_read_1();
        break;
    case s_fsm_main_read_temp_1:
        fsm_main_read_temp_1();
        break;
    case s_fsm_main_send_reset_2:
        fsm_main_send_reset_2();
        break;
    case s_fsm_main_setup_wb_rom_match_2:
        fsm_main_setup_wb_rom_match_2();
        break;
    case s_fsm_main_send_rom_match_2:
        fsm_main_send_rom_match_2();
        break;
    case s_fsm_main_send_serial_id_2:
        fsm_main_send_serial_id_2();
        break;
    case s_fsm_main_send_func_read_2:
        fsm_main_send_func_read_2();
        break;
    case s_fsm_main_read_temp_2:
        fsm_main_read_temp_2();
        break;
    case s_fsm_main_send_reset_3:
        fsm_main_send_reset_3();
        break;
    case s_fsm_main_setup_wb_rom_match_3:
        fsm_main_setup_wb_rom_match_3();
        break;
    case s_fsm_main_send_rom_match_3:
        fsm_main_send_rom_match_3();
        break;
    case s_fsm_main_send_serial_id_3:
        fsm_main_send_serial_id_3();
        break;
    case s_fsm_main_send_func_read_3:
        fsm_main_send_func_read_3();
        break;
    case s_fsm_main_read_temp_3:
        fsm_main_read_temp_3();
        break;
    case s_fsm_main_send_reset_4:
        fsm_main_send_reset_4();
        break;
    case s_fsm_main_setup_wb_rom_match_4:
        fsm_main_setup_wb_rom_match_4();
        break;
    case s_fsm_main_send_rom_match_4:
        fsm_main_send_rom_match_4();
        break;
    case s_fsm_main_send_serial_id_4:
        fsm_main_send_serial_id_4();
        break;
    case s_fsm_main_send_func_read_4:
        fsm_main_send_func_read_4();
        break;
    case s_fsm_main_read_temp_4:
        fsm_main_read_temp_4();
        break;
    case s_fsm_main_process_temps:
        fsm_main_process_temps();
        break;
    case s_fsm_start_transmission:
        fsm_start_transmission();
        break;
    case s_fsm_check_temp_1:
        fsm_check_temp_1();
        break;
    case s_fsm_check_temp_2:
        fsm_check_temp_2();
        break;
    case s_fsm_check_temp_3:
        fsm_check_temp_3();
        break;
    case s_fsm_check_temp_4:
        fsm_check_temp_4();
        break;
    case s_fsm_end_transmission:
        fsm_end_transmission();
        break;
    case s_fsm_wait_pause:
        fsm_wait_pause();
        break;
    }
}