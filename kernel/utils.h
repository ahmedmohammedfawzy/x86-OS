#pragma once
#include <stdint.h>

void memory_copy(char *source, char *dest, uint32_t nbytes);
void int_to_ascii(int32_t n, char str[]);
void reverse(char s[]);
int strlen(char s[]); 

uint16_t low_16(uint32_t bits);
uint16_t high_16(uint32_t bits);
