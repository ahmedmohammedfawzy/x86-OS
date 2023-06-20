#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "../cpu/timer.h"
#include "../kernel/utils.h"
#include "../drivers/keyboard.h"

void main() {
    clear_screen();
    isr_install();
    asm volatile("sti");

    init_keyboard();
}
