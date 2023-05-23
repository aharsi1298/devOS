#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include "../libc/mem.h"
#include "type.h"

#define STARTING_ADDR 0xC0000

void virt_to_phy(uint_ptr virt_addr);

void setup_paging();
void enable_paging();

#define PDE_4_PRESENT         0x1
#define PDE_4_READ_WRITE      0x2
#define PDE_4_USER_SUPERVISOR 0x4
#define PDE_4_PWT             0x8
#define PDE_4_PCD             0x10
#define PDE_4_ACCESSED        0x20
#define PDE_4_DIRTY           0x40
#define PDE_4_PS              0x80
#define PDE_4_GLOBAL          0x100
#define PDE_4_PAT             0x1000

#define PDE_PRESENT         0x1
#define PDE_READ_WRITE      0x2
#define PDE_USER_SUPERVISOR 0x4
#define PDE_PWT             0x8
#define PDE_PCD             0x10
#define PDE_ACCESSED        0x20
#define PDE_PS              0x80

#define PTE_PRESENT         0x1
#define PTE_READ_WRITE      0x2
#define PTE_USER_SUPERVISOR 0x4
#define PTE_PWT             0x8
#define PTE_PCD             0x10
#define PTE_ACCESSED        0x20
#define PTE_PS              0x80

#endif