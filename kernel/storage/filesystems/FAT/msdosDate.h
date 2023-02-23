//
// Created by nigel on 23/02/23.
//

#pragma once

#include "../../../terminal/kterm.h"

// Date Format
// [0..4] Day
// [5..8] Month
// [9..15] Year
class MSDOSDATE {
    static void ParseDate(unsigned int date){
        printf("Date (hex) 0x%x\n", date);
        unsigned int year = (date >> 9 )+ 1980;
        unsigned int month = (date & 0xf0   ) >> 4;
        unsigned int day = date & 0xf ;
        printf("Date: (D,M,Y) %d, %d ,%d\n", day , month, year );
    }
};



// Time Format
// [0..4] Seconds
// [5..10] Minute
// [11..15] Hour
class MSDOSTIME {
    static void ParseTime(unsigned int time)
    {
        printf("Time (hex) 0x%x\n", time);
        unsigned int seconds =  ( time & 0x0f) * 2;
        unsigned int minutes = (time & 0xf0);
        unsigned int hours = (time & 0xf00);
        printf("Time (H:M:S) %d:%d:%d\n", hours, minutes, seconds);
    }
};


