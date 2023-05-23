#include "acpi.h"
#include "../libc/string.h"

uint32_t *find_rsdp_addr() {
    uint8_t *ebda_ptr = 0x40E;
    uint8_t *ebda = (*ebda_ptr) * 16 + (*(ebda_ptr + 1));
    for(int i = 0; i < 1024; i += 16) {
        if(strcmp("RSD PTR ", ebda+i) == 0) {rsdp_addr = ebda + i; return ebda + i;}
    }
    uint8_t *loc = 0xE0000;
    for(int j = 0; j < 1024; j += 16) {
        if(strcmp("RSD PTR ", loc+j) == 0) {rsdp_addr = loc + j; return loc + j;}
    }
    rsdp_addr = 0;
    return 0;
}

int find_acpi_rev() {
    uint32_t revision = *(rsdp_addr+16);
    if(revision == 0) return 1;
    else if(revision == 2) return 2;
}

uint32_t *select_table() {
    int version = find_acpi_rev();
    if(version == 1) return *(rsdp_addr+20);
    else if(version == 2) {
        uint64_t *addr = *(rsdp_addr+28);
        uint32_t *casted_addr = (uint32_t *)addr;
        return casted_addr;
    }
}