#include "loop.h"
#include <stdlib.h>
#include "tusb.h"
#include "OLED_I2C.h"
#include "bsp/board.h"
#include "pico/multicore.h"

volatile bool loopStateFlag = false;

volatile uint8_t BPM = 60;
uint8_t played_note = NOTE_COUNT / 3 - 1;

void init_loop(){
    gpio_init(START_STOP_PIN);
    gpio_set_dir(START_STOP_PIN, GPIO_IN);
    gpio_pull_up(START_STOP_PIN);
}

int32_t bpm_to_us(uint8_t BPM) {
    return 60*1000000/BPM;
}

void core1_entry() {
    absolute_time_t next_beat = get_absolute_time();
    note* notes = get_notes();
    uint8_t n = NOTE_COUNT / 3 ;
    bool was_playing = false;

    while(true) {
        if (loopStateFlag) {
            if(time_reached(next_beat)){
                board_led_on();
                if(was_playing){
                    play_note(MIDI_NOTE_OFF, &notes[played_note], &notes[played_note + n], &notes[played_note + 2*n]);
                }

                played_note = ((played_note + 1) % get_loop_after()) % n;

                play_note(MIDI_NOTE_ON, &notes[played_note], &notes[played_note + n], &notes[played_note + 2*n]);

                multicore_fifo_push_blocking(played_note);
                next_beat = delayed_by_us(next_beat, bpm_to_us(BPM));
                sleep_ms(5);
                board_led_off();
                was_playing = true;
            }
        }
        else{
            if(was_playing){
                was_playing = false;
                play_note(MIDI_NOTE_OFF, &notes[played_note], &notes[played_note + n], &notes[played_note + 2*n]);
            }
            next_beat = get_absolute_time();
        }
        sleep_us(10);
    }
}

void play_note(uint8_t flag, note* note0, note* note1, note* note2) {
    uint8_t ReceivedPacketBytes[9] = {
        flag | 0, note0->pitch, note0->velocity,
        flag | 1, note1->pitch, note1->velocity, 
        flag | 2, note2->pitch, note2->velocity
    };
    tud_midi_stream_write(MIDI_CABLE, ReceivedPacketBytes, 9);
}

void start_stop_loop(){
    if (loopStateFlag){
        loopStateFlag = false;
        OLED_render_notes(get_notes(), get_current_note());
    }
    else{
        loopStateFlag = true;
        played_note = NOTE_COUNT / 3 - 1;
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
    show_bpm_for((BPM+19)/20);
}

uint8_t get_bpm(){
    return BPM;
}

uint8_t get_played_note(){
    return played_note;
}