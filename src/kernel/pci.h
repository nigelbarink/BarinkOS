#pragma once
#include <stdint.h>
#include "io.h"
// Configuration Space Access Mechanism #1
#define CONFIG_ADDRESS 0xCF8 // Configuration adress that is to be accessed
#define CONFIG_DATA 0xCFC // Will do the actual configuration operation

uint32_t ConfigReadWord (uint8_t bus, uint8_t device, uint8_t func, uint8_t offset);


inline uint16_t getVendorID(uint8_t bus, uint8_t device, uint8_t function ){
        return  ConfigReadWord ( bus , device, function, 0);
}

inline uint16_t getDeviceID(uint8_t bus, uint8_t device, uint8_t function ){
    return ConfigReadWord(bus, device, function , 16);
}
