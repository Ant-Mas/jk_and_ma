#pragma once

#include "notes.h"

#define I2C_DEFAULT PICO_DEFAULT_I2C_INSTANCE()
#define I2C_SDA_PIN 16
#define I2C_SCL_PIN 17


#define OLED_HEIGHT              64
#define OLED_WIDTH               128

#define OLED_I2C_ADDR            _u(0x3C)
#define OLED_I2C_CLK             400*1000

#define OLED_OFF        		   0xAE
#define OLED_ON         		   0xAF

#define OLED_COMMAND_MODE       0x00
#define OLED_ONE_COMMAND_MODE   0x80
#define OLED_DATA_MODE          0x40
#define OLED_ONE_DATA_MODE      0xC0

#define OLED_CONTRAST           0x81

#define OLED_NORMALDISPLAY      0xA6
#define OLED_INVERTDISPLAY      0xA7

#define OLED_NORMAL_V           0xC8
#define OLED_FLIP_V             0xC0
#define OLED_NORMAL_H           0xA1
#define OLED_FLIP_H             0xA0

#define OLED_CLOCKDIV           0xD5
#define OLED_SETMULTIPLEX       0xA8

#define OLED_CHARGEPUMP         0xAD		
#define OLED_ADDRESSING_MODE    0x20
#define OLED_VERTICAL           0x01
#define OLED_HORIZONTAL         0x00

#define OLED_SETVCOM_DESEL      0xDB
#define OLED_SETCOMPINS         0xDA
#define OLED_SET_OFFSET			0xD3

#define OLED_START_LINE			0x40
#define OLED_PRECHARGE			0xD9
#define OLED_SETVPP				0x32

#define OLED_FIRSTCOL_H         0x10
#define OLED_FIRSTCOL_L         0x02

#define OLED_NUM_PAGES			8
#define OLED_FIRSTPAGE          0xB0

#define OLED_BUF_LEN            OLED_WIDTH * OLED_NUM_PAGES
#define OLED_UNIT_BUFLEN        OLED_WIDTH

#define OLED_STRING_OFFSET    1


void OLED_send_cmd(uint8_t cmd);

void OLED_send_cmd_list(uint8_t *buf, int num);

void OLED_send_buf(uint8_t buf[]);

void OLED_init(void);

void OLED_clear_buf(void);

void OLED_flash(void);

void OLED_render(void);

void OLED_clear(void);

void OLED_draw_rect(void);

void OLED_set_pixel(int col, int page, bool on);

int OLED_get_font_index(uint8_t ch);

uint8_t OLED_reverse(uint8_t b);

void OLED_fill_reversed_cache(); 

void OLED_note_fill_reversed_cache();

void OLED_write_char(int16_t col, int16_t page, uint8_t ch);

void OLED_write_note(int16_t col, int16_t page, uint8_t ch);

void OLED_write_string(int16_t col, int16_t page, char *str);

void OLED_write_note_string(int16_t col, int16_t page, char *str);

void OLED_insert_note_cursor(uint8_t position);

void OLED_render_notes(note* notes, uint8_t current_note);

void OLED_render_params(note note, uint8_t current_param);

void show_bpm_for(uint8_t frames);