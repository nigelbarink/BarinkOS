#pragma once
#include <stdint.h>

/*
Based on Intel specifications.

C++ interface for the cpu.s assembly file.

©Nigel Barink - 2022 
*/


/*
*       EFLAGS FUNCTIONS
*/

extern "C" uint32_t GetEFLAGS();



/*
*       CONTROL_REGISTER_0 FUNCTIONS
*/
extern "C" uint32_t GetCR0();

/*
 struct CR0_Register  {
        uint8_t PE :1; // Protection Mode Enabled 0
        uint8_t MP :1; // Monitor co-processor 1
        uint8_t EM :1; // Emulation 2
        uint8_t TS :1; // Task switched 3
        uint8_t ET :1; // Extension Type 4
        uint8_t NE :1; // Numeric error 5
        uint16_t Reserved :10; // 6,7,8,9,10,11,12,13,14,15
        uint8_t WP :1; // Write Protect 16
        uint8_t Reserved :1; // 17
        uint8_t AM :1; // Alligment Task 18
        uint16_t Reserved :10; // 19,20,21,22,23,24,25,26,27,28
        uint8_t NW :1; // Not-write through 29
        uint8_t CD :1; // Cache disable 30
        uint8_t PG :1; // Paging 31
    };*/



#define GET_PE_BIT(CONTROL_REGISTER_0) (CONTROL_REGISTER_0&0x1)
#define GET_MP_BIT(CONTROL_REGISTER_0) (CONTROL_REGISTER_0&0x2)
#define GET_EM_BIT(CONTROL_REGISTER_0) (CONTROL_REGISTER_0&0x3)
#define GET_TS_BIT(CONTROL_REGISTER_0) (CONTROL_REGISTER_0&0x4)
#define GET_ET_BIT(CONTROL_REGISTER_0) (CONTROL_REGISTER_0&0x5)
#define GET_NE_BIT(CONTROL_REGISTER_0) (CONTROL_REGISTER_0&0x6)


#define GET_PG_BIT(CONTROL_REGISTER_0) (CONTROL_REGISTER_0>>31)

/*
*       CONTROL_REGISTER_4 FUNCTIONS
*/

extern "C" uint32_t GetCR4();

#define GET_PSE_BIT(CONTROL_REGISTER_4) (CONTROL_REGISTER_4&0x4)
#define GET_PAE_BIT(CONTROL_REGISTER_4) (CONTROL_REGISTER_4&0x5)

/*
*       CONTROL_REGISTER_2 FUNCTIONS
*/

extern "C" uint32_t GetCR2();

/*
*       CONTROL_REGISTER_3 FUNCTIONS
*/

extern "C" uint32_t GetCR3();