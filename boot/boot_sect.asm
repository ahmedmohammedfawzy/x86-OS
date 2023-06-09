; a boot sector that load a C kernel in 32-bit mode
[org 0x7c00]

KERNEL_OFFSET equ 0x1000

mov [BOOT_DRIVE], dl

mov bp, 0x9000
mov sp, bp

mov bx, MSG_REAL_MODE
call print_string

call load_kernel

call switch_to_pm ; We never return from here

jmp $

%include "print.asm"
%include "disk_load.asm"
%include "gdt.asm"
%include "pm-print.asm"
%include "pm-switch.asm"

[bits 16]
load_kernel:
    mov bx, MSG_KERNEL_LOAD
    call print_string

    mov bx, KERNEL_OFFSET
    mov dh, 25
    mov dl, [BOOT_DRIVE]
    call disk_load

    ret


[bits 32]
BEGIN_PM:
    mov ebx, MSG_PROT_MODE
    call print_string_pm

    call KERNEL_OFFSET

    jmp $

; Global variables
BOOT_DRIVE db 0
MSG_REAL_MODE db "Started in 16 - bit Real Mode " , 0
MSG_PROT_MODE db "Successfully landed in 32 - bit Protected Mode " , 0
MSG_KERNEL_LOAD db "Loading Kernel in memory", 0

; boot sector padding
times 510-($-$$) db 0
dw 0xaa55
