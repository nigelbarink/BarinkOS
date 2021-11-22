#include "kterm.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t kterm_row;
size_t kterm_column;
uint8_t kterm_color;
uint16_t* kterm_buffer;


static inline uint8_t vga_entry_color( enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

static inline uint16_t vga_entry (unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

void kterm_init () { 
    kterm_row = 0;
    kterm_column = 0;
    kterm_color = vga_entry_color ( VGA_COLOR_LIGHT_GREY , VGA_COLOR_BLACK);
    kterm_buffer = (uint16_t*) 0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++ ){
        for( size_t x = 0; x < VGA_WIDTH; x++){
            const size_t index = y * VGA_WIDTH + x;
            kterm_buffer[index] = vga_entry(' ', kterm_color);

        }
    }
}


void kterm_resetcolor(){
     kterm_color = vga_entry_color ( VGA_COLOR_LIGHT_GREY , VGA_COLOR_BLACK);
}

void kterm_setcolor(uint8_t color){
    kterm_color = color;
}

void kterm_putat (char c, uint8_t color, size_t x, size_t y ) {
    const size_t index = y * VGA_WIDTH + x;

    kterm_buffer[index] = vga_entry(c, color);

}

void enable_cursor (uint8_t start_cursor , uint8_t end_cursor ){
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | start_cursor);

    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | end_cursor);
}

void disable_cursor() 
{
        outb(0x3D4, 0x0A);
        outb(0x3D5, 0x20);
}


void update_cursor(int x, int y){
    uint16_t pos = y * VGA_WIDTH + x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

uint16_t get_cursor_position(){
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t) inb(0x3D5)) << 8;
    return pos;
}

int get_cursor_x (uint16_t cursor_pos) {
    return cursor_pos % VGA_WIDTH;
}

int get_cursor_y (uint16_t cursor_pos ) {
    return cursor_pos / VGA_WIDTH;
}



/**
 *  With the help from:
 * https://whiteheadsoftware.dev/operating-systems-development-for-dummies/
 **/ 
void kterm_scrollup(){
    size_t i ;
    for(i=0; i < (VGA_WIDTH * VGA_HEIGHT - VGA_WIDTH); i++)
        kterm_buffer[i] = kterm_buffer[i+VGA_WIDTH];

    for( i=0; i< VGA_WIDTH; i++)
        kterm_buffer[(VGA_HEIGHT -1) * VGA_WIDTH + i ] = vga_entry(' ', kterm_color); 

}

void kterm_put (char c) {
    if(++kterm_column == VGA_WIDTH || c == '\n' ) {
        update_cursor(kterm_column , kterm_row);
        kterm_column = 0;
        if(kterm_row == VGA_HEIGHT-1 ) {       
            kterm_scrollup();
        } else {
            kterm_row ++;
        }
    }
    if ( c == '\r'){
        kterm_column = 0;
        return;
    }
    
    if(c == '\n') return;
    kterm_putat ( c, kterm_color, kterm_column, kterm_row);
    

}

void kterm_write(const char* data, size_t size) {
    for(size_t i = 0; i < size; i++){
        kterm_put(data[i]);
    }
}   

void kterm_writestring(const char* data ){
   // AS_KERNEL();
    kterm_write(data, strlen(data));
}


void itoa (char *buf, int base, int d) {
    char *p = buf;
    char *p1, *p2;
    unsigned long ud = d;
    int divisor = 10;
    if ( base == 'd' && d < 0){
        *p++ = '-';
        buf++;
        ud = -d;
    } else if (base == 'x'){
        divisor = 16;
    }

    do { 
        int remainder = ud % divisor;

         *p++ = (remainder < 10 ) ? remainder + '0' : remainder + 'a' -10;
    } while(ud /= divisor);

    /*terminate buf*/
    *p =0;
    p1 = buf;
    p2 = p -1;

    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;

    }

}

void printf ( const char *format, ...) {
    return;
    char **arg = (char **)&format;
    int c;
    char buf[20];

    arg++;
    
    while ((c = *format++) != 0){
        if( c != '%')
            kterm_put(c);
        else{
            char *p, *p2;
            int pad0 = 0, pad = 0;

            c = *format++;
            if(c =='0'){
                pad0 = 1;
                c = *format++;
            }

            if ( c >= '0' && c <= '9'){
                pad = c - '0';
                c = *format++;
            }

            switch (c)
            {
            case 'd':
             
            case 'u':
            case 'x':
                itoa(buf, c, *((int *) arg++));

                p = buf;
                goto string;
                break;
            
            case 's':
                p = *arg++;
                if(!p)
                    p = "(null)";
                
                string: 
                    for (p2 = p; *p2; p2++);
                    for (; p2 < p + pad; p2++)
                        kterm_put(pad0 ? '0': ' ');
                    while (*p)
                        kterm_put(*p++);
                    break;


            default:
                kterm_put(*((int *)arg++));
                break;
            }
        }
    }
}