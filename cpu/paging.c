#include "paging.h"

void virt_to_phy(uint_ptr virt_addr) {
    uint_ptr pointer;
    asm volatile("movl %%cr3, %%eax" : "=a" (pointer) : );
    pointer = (*pointer) & 0xFFFFF000;
    pointer = ((*pointer) & 0xFFFFF000) + ((unsigned int)virt_addr & (unsigned int)0xFFF);
    return pointer;
}

uint32_t *page_directory = 0x8000;
uint32_t *first_page_table = 0x9000;

void setup_paging() {
    for(int i = 0; i < 1024; i++) {
        page_directory[i] = 0x00000002;
    }

    for(unsigned int j = 0; j < 1024; j++) {
        first_page_table[j] = (j * 0x1000) | 3;
    }

    page_directory[0] = ((unsigned int)first_page_table) | 3;
}

void enable_paging() {
    asm volatile("movl %%ebx, %%eax" : : "b" (page_directory));
    asm("movl %eax, %cr3");
    asm("movl %cr0, %eax");
    asm("orl $0x80000001, %eax");
    asm("movl %eax, %cr0");
}

/*
  0xC0000000 = 0b11000000000000000000000000000000
  0b1100000000 0000000000 000000000000
  0x300 0 0

  0x300 * 0x1000 + 0x11000 = 0x300000 + 0x11000 = 0x311000
  0x300 << 22 + 0 << 12 = 0b00000000000000000000001100000000
*/