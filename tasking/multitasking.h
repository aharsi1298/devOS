#ifndef MULTITASKING_H
#define MULTITASKING_H

#include <stdint.h>

typedef int (*thread)(int, char**, uint32_t, int);
typedef thread *process;

process *processes;

/*

QUIT = 0
DESTROY = 1
PAINT = 2
BUTTON_CLICKED = 3

*/



#endif