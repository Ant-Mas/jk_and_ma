#include "loop.h"
#include <stdlib.h>
#include "adc.h"
#include "tusb.h"
#include "OLED_I2C.h"
#include "bsp/board.h"
#include "pico/multicore.h"

volatile bool loopStateFlag = false;

volatile uint8_t BPM = 60;
uint8_t played_note = 0;

void init_loop(){
    gpio_init(START_STOP_PIN);
    gpio_set_dir(START_STOP_PIN, GPIO_IN);
    gpio_pull_up(START_STOP_PIN);
}

int32_t bpm_to_us(uint8_t BPM) {
    return 60*1000000/BPM;
}

void core1_entry(){
    absolute_time_t next_beat = get_absolute_time();
    note* notes = get_notes();
    uint8_t n = NOTE_COUNT / 3;

    while(true){
        if (loopStateFlag) {
            if(time_reached(next_beat)){
                board_led_on();
                sleep_ms(10);
                board_led_off();
                for(int i=0; i<3; i++){
                    play_note(MIDI_NOTE_OFF, i, &notes[played_note + i*n]);
                }

                played_note = ((played_note + 1) % get_loop_after()) % n;

                for(int i=0; i<3; i++){
                    play_note(MIDI_NOTE_ON, i, &notes[played_note + i*n]);
                }

                multicore_fifo_push_blocking(played_note);
                // OLED_render_notes(notes, played_note); // Nie powinno tu być
                // BPM = scan_adc(); // SHOULDNT BE HERE
                next_beat = delayed_by_us(next_beat, bpm_to_us(BPM));
            }
        }
        else{
            next_beat = get_absolute_time();
        }
        sleep_us(10);
    }
}

void play_note(uint8_t flag, uint8_t channel, note* note) {
    uint8_t ReceivedPacketBytes[3] = {flag | channel, note->pitch, note->velocity};
    tud_midi_stream_write(MIDI_CABLE, ReceivedPacketBytes, 3);
}

void start_stop_loop(){
    if (loopStateFlag){
        loopStateFlag = false;
        OLED_render_notes(get_notes(), get_current_note());
    }
    else{
        loopStateFlag = true;
        played_note = 0;
        // add_alarm_in_us(10, timer_callback, NULL, false);
    }
}

bool get_loop_state_flag(){
    return loopStateFlag;
}

void change_bpm(int8_t value){
    if(BPM + value < 30){
        BPM = 30;
        return;
    }
    if(BPM + value > 255){
        BPM = 255;
        return;
    }
    BPM = BPM + value;
}

uint8_t get_bpm(){
    return BPM;
}

uint8_t get_played_note(){
    return played_note;
}