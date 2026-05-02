#pragma once
#include "notes.h"
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define MIDI_CABLE 0
#define START_STOP_PIN 2

#define MIDI_NOTE_ON 0x90
#define MIDI_NOTE_OFF 0x80
#define MIDI_CC 0xB0

void init_loop();

void play_note(uint8_t flag, uint8_t channel, note* note);

int32_t bpm_to_us(uint8_t BPM); 

void core1_entry();

void start_stop_loop();

bool get_loop_state_flag();

void change_bpm(int8_t value);

uint8_t get_bpm();

uint8_t get_played_note();