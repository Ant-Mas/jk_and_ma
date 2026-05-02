#pragma once
#include <stdint.h>
#include <stdbool.h>

#define NOTE_COUNT 48
#define PARAM_COUNT 3

typedef struct {
    uint8_t pitch;
    uint8_t velocity;
} note;

void encoder_move(bool down);
void switch_press(void);
uint8_t* get_nth(note *note, uint8_t n);

char decode(note note);

note* get_notes(void);

void init_notes(uint8_t pitch, uint8_t velocity);

char* pitch_to_str(uint8_t pitch);

uint8_t get_loop_after();

uint8_t get_current_note();