//
// Created by nigel on 17/02/23.
//
#pragma  once
#include "../terminal/kterm.h"
class processor {
public:
    static void initialize();

    // Based on information from https://en.wikichip.org/wiki/x86/amx#Detection
    enum  AMX_TYPE{
        AMX_BF16 = (0x1 << 22),
        AMX_TILE = (0x1 << 24),
        AMX_INT8 = (0x1 << 25)
    };
    static bool hasAMXExtension();
    static bool has32bitPagingSupport();
    static bool hasPageSupport();
    static bool gigabytePages();

    static bool hasPAEExtension();
    static void enable_protectedMode();

    static uint32_t GetEFLAGS();
    static uint32_t GetCR0();
    static uint32_t GetCR2();
    static uint32_t GetCR3();
    static uint32_t GetCR4();

private:
    static uint32_t cap_page;
    static uint32_t cap_page1;
    static uint32_t cap_page7;


};
