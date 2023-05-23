#include "timer.h"
#include "../libc/function.h"
#include "isr.h"
#include "ports.h"

int tick = 0;

static void timer_callback(registers_t *regs) {
    tick++;
    /*kprint_at("Tick: ", 0, 0);

    char tick_ascii[256];
    int_to_ascii(tick, tick_ascii);
    kprint_at(tick_ascii, 0, 6);
    kprint("\n");*/
    UNUSED(regs);
}

void init_timer(uint32_t freq) {
    register_interrupt_handler(IRQ0, &timer_callback);

    uint32_t divisor = 1193180 / freq;
    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    port_byte_out(0x43, 0x36); // Hey, I got something for you
    port_byte_out(0x40, low); // Here's the first
    port_byte_out(0x40, high); // Here's the second
}