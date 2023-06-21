#include <stdint.h>

void memory_copy(char* source, char *dest, uint32_t nbytes);
uint16_t low_16(uint32_t bits);
uint16_t high_16(uint32_t bits);
uint32_t malloc(uint32_t size, int align, uint32_t* phys_addr);
