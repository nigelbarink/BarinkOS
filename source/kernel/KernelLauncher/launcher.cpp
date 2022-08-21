#include <stdint.h>
#include <stddef.h>

void put_char(char ch, size_t x, size_t y ){
    *((uint16_t*)0xb8000+(y * 80 + x)) = ch | ((7 | 0 << 4) << 8);
}

void write_ln(char* s, size_t length, size_t x , size_t y)
{
  // Because read only data is linked at a virtual address we'll need to convert
  // the string adddres from virtual to phys.
  s = s - 0xC0000000;
  size_t column , row;
  column = x;
  row = y;
  for(int i = 0; i < length ; i ++)
  {
    
    put_char(s[i] , column,row );
    column ++;

  }
  
}

extern "C" void testLauncher  () {
  write_ln("hello", 5 ,0,0);
}