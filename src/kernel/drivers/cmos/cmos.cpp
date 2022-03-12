void ReadFromCMOS(unsigned char array[])
{
    unsigned char tvalue, index;

    for (index = 0; index < 128; index++)
    {
        asm(
            "cli\n\t"           // Disable interrupts
            "mov al, index\n\t" // Move index address
            // since the 0x80 bit of al is not set, NMI is active 
            "out 0x70,al\n\t" // Copy address to CMOS register
            // some kind of real delay here is probably best 
            "in al,0x71\n\t" // Fetch 1 byte to al
            "sti\n\t"        // Enable interrupts
            "mov tvalue,al\n\t");

        array[index] = tvalue;
    }
}

void WriteTOCMOS(unsigned char array[])
{
   unsigned char index;
 
   for(index = 0; index < 128; index++)
   {
      unsigned char tvalue = array[index];
      
       asm("cli\n\t"       // Clear interrupts
         "mov al,index\n\t"    // move index address
         "out 0x70,al\n\t"     // copy address to CMOS register
         // some kind of real delay here is probably best 
         "mov al,tvalue\n\t"   // move value to al
         "out 0x71,al\n\t"   // write 1 byte to CMOS
         "sti\n\\t"   );        // Enable interrupts
      
   }
}