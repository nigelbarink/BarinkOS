#pragma once 
#include <stdint.h>


void initHeap();

void* malloc (size_t size );
void free(void* addr);

