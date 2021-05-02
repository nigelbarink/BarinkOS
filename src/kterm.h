#include <stdint.h>
#include <stddef.h>
#include "vga_colors.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t kterm_row;
size_t kterm_column;
uint8_t kterm_color;
uint16_t* kterm_buffer;
