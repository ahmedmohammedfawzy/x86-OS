#include <stdint.h>

void memory_copy(char* source, char* dest, uint32_t nbytes) 
{
    for (int i = 0;i < nbytes; i++) 
    {
        *(source + i) = *(dest + i);
    }
}

void memory_set(uint8_t *dest, uint8_t val, uint32_t len) {
    uint8_t *temp = (uint8_t *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}


uint16_t low_16(uint32_t bits) 
{
    return (bits & 0xFFFF);
}

uint16_t high_16(uint32_t bits) 
{
    return ((bits >> 16) & 0xFFFF);
}

/* This should be computed at link time, but a hardcoded
 * value is fine for now. Remember that our kernel starts
 * at 0x1000 as defined on the Makefile */
/* Implementation is just a pointer to some free memory which
 * keeps growing */
uint32_t malloc(uint32_t size, int align, uint32_t* phys_addr) {
    static uint32_t free_mem_addr = 0x10001;
    /* Pages are aligned to 4K, or 0x1000 */
    if (align == 1 && (free_mem_addr & 0xFFFFF000)) {
        free_mem_addr &= 0xFFFFF000;
        free_mem_addr += 0x1000;
    }
    /* Save also the physical address */
    if (phys_addr) *phys_addr = free_mem_addr;

    uint32_t ret = free_mem_addr;
    free_mem_addr += size; /* Remember to increment the pointer */
    return ret;
}
