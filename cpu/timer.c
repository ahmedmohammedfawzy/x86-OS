#include "timer.h"
#include "../drivers/screen.h"
#include "isr.h"
#include "../drivers/port.h"
#include "../libc/string.h"
#include <stdint.h>

uint32_t tick = 0;

static void timer_callback(registers_t regs) {
//    print("timer_callback called\n");
    tick++;
//    print("Tick: ");
    
    char tick_ascii[256];
    int_to_ascii(tick, tick_ascii);
    print(tick_ascii);
    print("\n");
}

void init_timer(uint32_t frequency)
{
    register_interrupt_handler(IRQ0, timer_callback);

    /* Get the PIT value: hardware clock at 1193180 Hz */
    uint32_t divisor = 1193180 / frequency;
    uint8_t low  = divisor & 0xFF;
    uint8_t high = ( (divisor >> 8) & 0xFF);
    /* Send the command */
    port_byte_out(0x43, 0x36); /* Command port */
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);

}
