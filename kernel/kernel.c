#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "kernel.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include "../cpu/paging.h"
#include "../libc/function.h"

extern void enable_paging(void);

int *drivers[];

void kernel_main(int cpuid, int pci_mech1) {
    // Enable paging
    setup_paging();
    enable_paging();
    // noo

    clear_screen();
    /* Don't look here
    char *secret_message = "I love you mamma. Mmuaa *kiss*\nMamma loves Picuti to the power of infinity\nSwimming taim!\npoopy time\npaglu time";
    kprint_at(secret_message, 0, 0);
    */
    isr_install();
    irq_install();

    kprint("devOS (C) CC-BY-NC\n");
    kprint("~ ");
    UNUSED(cpuid);
}

void user_input(char input[]) {
    if(strcmp(input, "EXIT") == 0) {
        kprint("Shutting down...");
        kprint("Shut down");
        asm volatile("outw %%ax, %%dx" : : "d" (0x2000), "a" (0xB004));
    } else if(strcmp(input, "CLEAR") == 0) {
        clear_screen();
        kprint("~ ");
    } else if(strcmp(input, "HELP") == 0) {
        kprint("Available commands:\n");
        kprint("CLEAR: Clears the screen\n");
        kprint("HELP: Shows you all the commands and what they do\n");
        kprint("EXIT: Shuts down the computer\n");
        kprint("~ ");
    } else if(strcmp(input, "PAGE") == 0) {
        uint32_t phys_addr;
        uint32_t page = kmalloc(1000, 1, &phys_addr);
        char page_str[16] = "";
        hex_to_ascii(page, page_str);
        char phys_str[16] = "";
        hex_to_ascii(phys_addr, phys_str);
        kprint("Page: ");
        kprint(page_str);
        kprint(", physical address: ");
        kprint(phys_str);
        kprint("\n~ ");
    }
     else {
        kprint("\"");
        kprint(input);
        kprint("\" is not recognised as an operable command. Use the \"help\" command to find out about more commands.\n");
        kprint("~ ");
    }
}