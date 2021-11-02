#pragma once

// Let's write an ATA PIO | ATA driver for now. Mostly to show that I can in theory interact with a
// storage device

// PRIMARY_ATA_BUS
// 0x1F0 through 0x1F7 

// SECONDARY_ATA_BUS
// 0x170 through 0x177

#define DEVICE_CONTROL_REGISTER 0x3F6
#define DEVICE_CONTROL_ALTERNATE 0x376


// IRQ14 Primary bus interrupt
// IRQ15 Secondary bus interrupt
