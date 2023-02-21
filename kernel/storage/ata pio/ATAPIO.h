#pragma once
#include <stdint-gcc.h>
#include "../ide/ideCommands.h"
#include "../ide/sampleIDE.definitions.h"
#include "../../devices/BlockDevice.h"
#include "../../terminal/kterm.h"

/*
* This first driver wil make use of IO ports.
* Doing so means reading or writing from disk is going
* to be very cpu intensive.
*/

enum DEVICE_DRIVE{
    MASTER = 0xA0,
    SLAVE = 0xB0
};


enum  ATAPIO_PORT {
    Primary = 0x1f0,
    Secondary = 0x170
};



class ATAPIO
{
public:
    static bool Identify(ATAPIO_PORT, DEVICE_DRIVE);
    static void Read (uint16_t, DEVICE_DRIVE, uint32_t, uint16_t*);
    static void Write(uint16_t, DEVICE_DRIVE);
    static void Soft_Reset(ATAPIO_PORT , DEVICE_DRIVE );
};


