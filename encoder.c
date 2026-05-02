#include "encoder.h"

#include "pico/stdlib.h"


void init_encoder() {

    gpio_init(ENCODER_PIN_A);
    gpio_init(ENCODER_PIN_B);

    gpio_set_dir(ENCODER_PIN_A, GPIO_IN);
    gpio_set_dir(ENCODER_PIN_B, GPIO_IN);

    gpio_pull_up(ENCODER_PIN_A);
    gpio_pull_up(ENCODER_PIN_B);

    gpio_init(SWITCH_PIN);
    gpio_set_dir(SWITCH_PIN, GPIO_IN);
    gpio_pull_up(SWITCH_PIN);
}

