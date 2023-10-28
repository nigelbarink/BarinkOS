//
// Created by nigel on 10/28/23.
//
#include "klog.h"
#include "terminal/kterm.h"
#include <CoreLib/Memory.h>

const char* ForeGroundColourReset = "\e[39m";
void print_dbg(const char* message, ...){
    auto **arg = (unsigned char**)&message;
    // Send it to the VGA
    printf(message, arg);

    // Now send the message to the serial
    Serial com1= Serial({
                        COM1,
                        0x03,
                        0x00
                });

    const char* ForeGroundColour = "\e[32m";
    com1.write((void*)ForeGroundColour, strlen(ForeGroundColour));
    com1.write((void*)message, strlen(message));
    com1.write((void*)ForeGroundColourReset, strlen(ForeGroundColourReset));

}


void print_info(const char* message, ...){
    auto **arg = (unsigned char**)&message;
    // Send it to the VGA
    printf(message, arg);

    Serial com1 = Serial({
        COM1,
        0x03,
        0x00
    });

    const char* ForeGroundColour = "\e[34m";
    com1.write((void*)ForeGroundColour, strlen(ForeGroundColour));
    com1.write((void*)message, strlen(message));
    com1.write((void*)ForeGroundColourReset, strlen(ForeGroundColourReset));
}
void print_err(const char* message, ...){
    auto **arg = (unsigned char**)&message;
    // Send it to the VGA
    printf(message, arg);
    Serial com1 = Serial({
                                 COM1,
                                 0x03,
                                 0x00
                         });
    const char* ForeGroundColour = "\e[31m";
    com1.write((void*)ForeGroundColour, strlen(ForeGroundColour));
    com1.write((void*)message, strlen(message));
    com1.write((void*)ForeGroundColourReset, strlen(ForeGroundColourReset));
}
