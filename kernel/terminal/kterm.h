#pragma once 
#include <stdint-gcc.h>
#include <stddef.h>
#include <stdbool.h>

#include "../drivers/vga/colors.h"
#include "../io/io.h"
#include <CoreLib/Memory.h>

void kterm_init();

/* Kernel terminal - Colour functions*/
void kterm_resetcolor();
void kterm_setcolor(uint8_t);

/* Kernel terminal - Printing function */
void kterm_putat(char, uint8_t, size_t, size_t);
void kterm_put(char);
void kterm_write(const char*, size_t);
void kterm_writestring(const char*);
void kterm_scrollup();


/* Kernel terminal - Cursor functions */
void enable_cursor (uint8_t start_cursor , uint8_t end_cursor );
void disable_cursor();
void update_cursor(int x, int y);
uint16_t get_cursor_position();
int get_cursor_x (uint16_t cursor_pos);
int get_cursor_y (uint16_t cursor_pos);


void printf ( const char *format, ...);

