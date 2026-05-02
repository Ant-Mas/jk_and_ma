#include "adc.h"
#include "hardware/adc.h"

uint8_t map(uint16_t x) {
    uint8_t result = x / 16;
    return result;
}

void init_adc(void) {
    adc_init();
    adc_gpio_init(ADC_PIN);
    adc_select_input(0);
}
    
uint8_t scan_adc(void) {
    uint8_t potValue = map(adc_read());
    return potValue;
}