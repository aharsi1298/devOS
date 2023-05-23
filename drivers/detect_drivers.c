/*
#include "detect_drivers.h"
#include "../cpu/ports.h"

void checkDevice(uint8_t bus, uint8_t device) {
    uint8_t function = 0;

    uint16_t vendorID = getVendorID(bus, device, function);
    if(vendorID == 0xFFFF) return;
    checkFunction(bus, device, function);
    uint16_t headerType = getHeaderType(bus, device, function);
    if((headerType & 0x80) != 0) {
        for(function = 1; function < 8; function++) {
            if(getVendorID(bus, device, function) != 0xFFFF) {
                checkFunction(bus, device, function);
            }
        }
    }
}

void checkFunction(uint8_t bus, uint8_t device, uint8_t function) {}

uint16_t pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint8_t mech1) {
    uint16_t tmp = 0;
    if(mech1 == 1) {
    uint32_t address;
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;

    address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0b11111100) | ((uint32_t)0x80000000));

    port_word_out(0xCF8, address);

    tmp = (uint16_t)((port_word_in(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);
    } else if(mech1 == 0) {
        //
    }
    return tmp;
}

void loadAllPciDevices() {

}
*/