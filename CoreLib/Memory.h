#pragma once

#include <stddef.h>
#include <stdint.h>

void* memset (void* ptr, int value, size_t num);

int memcmp( const void* ptr1, const void* ptr2, size_t num);

void memcpy (void* dest, const void* src, size_t count );

size_t strlen(const char* str);

int strncmp ( const char* str1, const char* str2, size_t num );

char* strtok(char* str, const char* delim , char**saveptr);