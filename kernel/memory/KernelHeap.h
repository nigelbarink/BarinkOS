#pragma once 
#include <stddef.h>
#include <stdint.h>
#include "../terminal/kterm.h"

void initHeap();

void* malloc (size_t size );
void free(void* addr);

