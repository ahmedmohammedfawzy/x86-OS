#include "idt.h"
#include "../kernel/utils.h"
#include <stdint.h>

idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

void set_idt_gate(uint8_t n, uint32_t handler)
{
    idt[n].low_offset = low_16(handler);
    idt[n].sel = KERNEL_CS;
    idt[n].always0 = 0;
    idt[n].flags = 0x8E;
    idt[n].high_offset = high_16(handler);
}

void set_idt() 
{
    idt_reg.base = (uint32_t)&idt;
    idt_reg.limit = sizeof(idt_gate_t) * IDT_ENTRIES - 1;
    asm volatile ("lidt [%0]" : : "r" (&idt_reg));
}
