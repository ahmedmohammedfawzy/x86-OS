#include "../cpu/isr.h"
#include "../drivers/screen.h"

void main() {
    clear_screen();
    isr_install();
    asm volatile("int 2");
    asm volatile("int 3");
}
