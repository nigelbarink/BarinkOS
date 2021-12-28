#pragma once
#include <stdint.h>
#include "../tty/kterm.h"
typedef enum ScanCodeSet{
    None            =   0,
    ScanCodeSet1    =   1,
    ScanCodeSet2    =   2,
    ScanCodeSet3    =   3,
};

typedef enum Modifiers{
    LSHIFT  =   1,
    RSHIFT  =   2,

    LCTRL   =   3,
    RCTRL   =   4,

    LALT    =   5,
    RALT    =   6
};

struct KeyPressInfo{ 
    uint8_t PressedModifiers;
    uint8_t ScanCode;
};



extern KeyPressInfo keyPress;

void KeyHandled();

char getASCIIKey();
uint8_t getKey();
