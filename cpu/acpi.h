#ifndef ACPI_H
#define ACPI_H

#include <stdint.h>

uint32_t *rsdp_addr;

uint32_t *find_rsdp_addr();
int find_acpi_rev();
uint32_t *select_table();

#endif