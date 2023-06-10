disk_load:
    push dx ; dh contains the number of sectors to read, so we push dx

    mov ah, 0x02 ; bios read sector function
    mov al, dh ; read dh sectors
    mov ch, 0x00 ; cylinder 0
    mov dh, 0x00 ; head 0
    mov cl, 0x02 ; start reading from second sector i.e after the boot sector 
    int 0x13 ; bios intruppt to read

    jc disk_error

    pop dx
    cmp dh, al
    jne disk_error
    ret

disk_error:
    mov bx, DISK_ERROR_MSG
    call print_string
    jmp $

DISK_ERROR_MSG db "Disk read error!", 0
