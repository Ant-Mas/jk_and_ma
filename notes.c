#include "notes.h"
#include <stddef.h>
#include "OLED_I2C.h"
#include "loop.h"

note notes[NOTE_COUNT];
uint8_t current_note = 0;
uint8_t current_param = 0;
uint8_t loop_after = 16;
bool choosing_note = false;

uint8_t* get_nth(note *note, uint8_t n){
    switch (n) {
        case 0:
            return &note->pitch;
        case 1:
            return &note->velocity;
        default:
            return (uint8_t*)0;
    }
}

void encoder_move(bool down) {
    int value;
    if(down) {
        value = 1;
    }
    else {
        value = -1;
    }

    if(get_loop_state_flag()){
        change_bpm(5 * value);
        OLED_render_notes(notes, get_played_note());
    }
    else if(choosing_note){
        if(current_param == 0){
            notes[current_note].pitch = (uint8_t)((notes[current_note].pitch + value - 11) % 25 + 36);
        }
        else if (current_param == 1) {
            notes[current_note].velocity = (uint8_t)((notes[current_note].velocity + value + 128) % 128);
        }
        else if (current_param == 2){
            loop_after = 1 + (loop_after - 1 + value + (NOTE_COUNT / 3)) % (NOTE_COUNT / 3);
        }
        OLED_render_params(notes[current_note], current_param);
    }
    else{
        current_note = (current_note + NOTE_COUNT + value) % NOTE_COUNT;
        OLED_render_notes(notes, current_note);
    }
}

char decode(note note){
    return ((note.pitch - 36) * 7 / 25) * 4 + note.velocity / 32 + 1;
}

void switch_press(void){
    if(get_loop_state_flag()){
        change_bpm(1);
    }
    else if(choosing_note){
        current_param += 1;
        if(current_param == PARAM_COUNT){
            // notes[current_note] = temp;

            choosing_note = false;
            OLED_render_notes(notes, current_note);
            return;
        }
        OLED_render_params(notes[current_note], current_param);
    }
    else{
        choosing_note = true;
        current_param = 0;
        OLED_render_params(notes[current_note], current_param);
    }

}

note* get_notes(void) {
    return notes;
}

void init_notes(uint8_t pitch, uint8_t velocity){
    pitch = ((pitch - 11) % 25 + 36);
    velocity = velocity % 128;
    for(int i=0;i<NOTE_COUNT; i++){
        notes[i].pitch = pitch;
        notes[i].velocity = velocity;
    }
}

char* pitch_to_str(uint8_t pitch){
    static char str[4];
    str[3] = 0;
    str[1] = ' ';
    str[2] = '0' + pitch/12 - 1;
    pitch = (pitch - 36) % 12;
    switch (pitch)
    {
    case 0:
        str[0] = 'C';
        break;
    case 1:
        str[0] = 'C';
        str[1] = '#';
        break;
    case 2:
        str[0] = 'D';
        break;
    case 3:
        str[0] = 'D';
        str[1] = '#';
        break;
    case 4:
        str[0] = 'E';
        break;
    case 5:
        str[0] = 'F';
        break;
    case 6:
        str[0] = 'F';
        str[1] = '#';
        break;
    case 7:
        str[0] = 'G';
        break;
    case 8:
        str[0] = 'G';
        str[1] = '#';
        break;
    case 9:
        str[0] = 'A';
        break;
    case 10:
        str[0] = 'A';
        str[1] = '#';
        break;
    case 11:
        str[0] = 'H';
        break;
    default:
        break;
    }
    return str;
}

uint8_t get_loop_after(){
    return loop_after;
}

uint8_t get_current_note(){
    return current_note;
}