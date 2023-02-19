#pragma once

#include <stddef.h>
#include <stdint.h>

void* memset (void* ptr, int value, size_t num);

int memcmp( const void* ptr1, const void* ptr2, size_t num);

size_t strlen(const char* str);

int strncmp ( const char* str1, const char* str2, size_t num );