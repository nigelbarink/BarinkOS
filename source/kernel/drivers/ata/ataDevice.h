#pragma once
#include <stdint.h>
#include "../io/io.h"
#include "../ide/ideCommands.h"
#include "../ide/sampleIDE.definitions.h"

#include "../../terminal/kterm.h"

/*
* This first driver wil make use of IO ports.
* Doing so means reading or writing from disk is going
* to be very cpu intensive.
* 
*/

enum DEVICE_DRIVE{
    MASTER = 0xA0,
    SLAVE = 0xB0
};




namespace ATA_DEVICE{
    void Identify(uint16_t, DEVICE_DRIVE);
    void Read (uint16_t, DEVICE_DRIVE, uint32_t, uint16_t*);
    void Write(uint16_t, DEVICE_DRIVE);
    void Soft_Reset(uint8_t ,DEVICE_DRIVE );
};


