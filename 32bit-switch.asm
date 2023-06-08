[bits 16]

switch_to_pm:
    cli ; disable interrupts
    lgdt [gdt_descriptor] ; load the gdt descriptor
    mov eax, cr0 
    or eax, 0x1 ; set the first bit in the cr0 register
    mov cr0, eax
    jmp CODE_SEG:init_pm ; issue a far jump to clear the pipeline and flush any 16-bit instructions

[bits 32]
; initialize the registers and the stack in 32 bit
init_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000
    mov esp, ebp

    call BEGIN_PM
    

