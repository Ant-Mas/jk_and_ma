#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "bsp/board.h"
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "pico/multicore.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"

#include "OLED_I2C.h"
#include "encoder.h"
#include "notes.h"
#include "adc.h"
#include "loop.h"

#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

volatile bool switchPressedFlag = false;
volatile bool encoderMovedFlag = false;
volatile bool startStopPressedFlag = false;
volatile bool encoderDirection;

void gpio_callback(uint gpio, uint32_t events) {
    
    if (gpio == SWITCH_PIN)
        switchPressedFlag = true;
    else if (gpio == START_STOP_PIN)
        startStopPressedFlag = true;
    else
        encoderMovedFlag = true;
        encoderDirection = gpio_get(ENCODER_PIN_B) == 0;
}

void switch_task() {
    switchPressedFlag = false;
    switch_press();
}

void start_stop_task() {
    startStopPressedFlag = false;
    start_stop_loop();
}

void encoder_task() {
    encoderMovedFlag = false;
    encoder_move(encoderDirection);
}

int main()
{
    board_init();
    stdio_init_all();
    tusb_init();

    OLED_init();
    init_encoder();
    init_adc();
    init_loop();
    init_notes(36, 0);
    uint8_t played_note;
    note* notes = get_notes();

    sleep_ms(500);

    multicore_launch_core1(&core1_entry);

    gpio_set_irq_enabled_with_callback(SWITCH_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(START_STOP_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(ENCODER_PIN_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    encoder_move(true);
    

    while (true) {
        tud_task();
        if (switchPressedFlag) switch_task();
        if (encoderMovedFlag) encoder_task();
        if (startStopPressedFlag) start_stop_task();
        if (multicore_fifo_rvalid()) {
            played_note = multicore_fifo_pop_blocking();
            OLED_render_notes(notes, played_note);
        }
    }
}
