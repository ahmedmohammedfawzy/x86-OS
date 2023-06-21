#include <stdint.h>

uint8_t port_byte_in(uint16_t port) {
    uint8_t result;
    asm("in al, dx" :  "=a" (result) : "d" (port));
    return result;
}

void port_byte_out(uint16_t port, uint8_t data) {
    asm("out dx, al" : :"a" (data), "d" (port));
}


uint16_t port_word_in(uint16_t port) {
    uint16_t result;
    asm("in ax, dx" :  "=a" (result) : "d" (port));
    return result;
}

void port_word_out(uint16_t port, uint16_t data) {
    asm("out dx, ax" : :"a" (data), "d" (port));
}
