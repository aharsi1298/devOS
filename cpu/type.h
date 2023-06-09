#ifndef TYPES_H
#define TYPES_H

#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

typedef signed int *int_ptr;
typedef unsigned int *uint_ptr;

#endif