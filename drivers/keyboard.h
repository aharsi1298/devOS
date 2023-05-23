#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include "../cpu/isr.h"

void init_keyboard();

char scancode_to_key(int scancode);

//typedef char* (keyboard_driver)(int, registers_t*);

#endif