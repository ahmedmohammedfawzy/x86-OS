#include "utils.h"
#include <stdint.h>

void memory_copy(char* source, char* dest, uint32_t nbytes) 
{
    for (int i = 0;i < nbytes; i++) 
    {
        *(source + i) = *(dest + i);
    }
}

void int_to_ascii(int32_t n, char str[])
{
    int sign;
    if ((sign = n) < 0) n = -n;

    int i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';
}

uint16_t low_16(uint32_t bits) 
{
    return (bits & 0xFFFF);
}

uint16_t high_16(uint32_t bits) 
{
    return ((bits >> 16) & 0xFFFF);
}
