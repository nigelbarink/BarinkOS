#include "keyboard.h"

KeyPressInfo keyPress {};

void KeyHandled(){
    keyPress.ScanCode= 0x00;
    keyPress.PressedModifiers = 0x00;
}


char getASCIIKey(){
    char keyPressed;
    // Wait until a key is pressed  
    while(keyPress.ScanCode == 0x00) {
        asm volatile ("NOP");
    }

    
    // Translate keycode to ascii 
    // Probably a lookup table might be handy 
    // Until 0x37
     const char* ASCIILookUp =
      "\01234567890-=\0\0QWERTYUIOP[]\0\0ASDFGHJKL;\'`\0\\ZXCVBNM,./\0"; 
     
     uint8_t ASCII_Index = keyPress.ScanCode - 3  ;
     //printf("ASCII_INDEX: %x\n", ASCII_Index);
     keyPressed = ASCIILookUp[ASCII_Index];

    KeyHandled();

    return keyPressed;
}



uint8_t getKey(){
    // Wait until a key is pressed 
    while(keyPress.ScanCode == 0x00){
        asm volatile ("NOP");
    }
    
    if( keyPress.ScanCode > 0x37){
        keyPress.ScanCode = 0x00;
        return 0;
    }

    uint8_t ScanCode = keyPress.ScanCode;
   // KeyHandled();

    return  ScanCode ;
}