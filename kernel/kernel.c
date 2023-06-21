#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "kernel.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include <stdint.h>

void kernel_main() {
    clear_screen();
    isr_install();
    irq_install();

    print("Type something, it will go through the kernel\n" 
          "Type END to halt the CPU\n> ");
}

void user_input(char* input)
{
    if (strcmp(input, "END") == 0) {
        print("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    } 
    else if (strcmp(input, "PAGE") == 0) 
    {
        uint32_t phys_addr;
        uint32_t page = malloc(1000, 1, &phys_addr);
        char page_str[16] = "";
        hex_to_ascii(page, page_str);
        char phys_str[16] = "";
        hex_to_ascii(phys_addr, phys_str);
        print("Page: ");
        print(page_str);
        print(", physical address: ");
        print(phys_str);
        print("\n");
    }
    print("You said: ");
    print(input);
    print("\n> ");
}


