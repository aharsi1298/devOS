#ifndef IDT_H
#define IDT_H

#include <stdint.h>
#include "type.h"

#define KERNEL_CS 0x08

typedef struct {
    uint16_t low_offset;     // Lower 16 bits of offset to isr
    uint16_t sel;            // Segment selector of kernel
    uint8_t always0;         // No need to explain (Reserved)
    uint8_t flags;
    /*  Flags:

        7   : Present bit, needs to be 1 to work
        6-5 : Privilege level needed to call the interrupt
        4   : Always 0 (0 means it's an interrupt gate)
        3-0 : The type of interrupt this is, can be task gate, 16 and 32bit interrupt gate and 16 and 32 bit trap gate
    */
    uint16_t high_offset;    // Higher 16 bits of offset to isr
} __attribute__((packed)) idt_gate_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_register_t;

#define IDT_ENTRIES 256 // Any CPU manufacturers mad enough to use my OS, and have a different number for this, change this

void set_idt_gate(int n, uint32_t handler);
void set_idt();

#endif