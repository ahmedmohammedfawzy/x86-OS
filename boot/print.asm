print_string:
    pusha


start:
    mov al, [bx] ; first we mov the contents of bx which is a pointer to al
    cmp al, 0 ; then we check if we reached the null terminator
    je done ; if so terminate routine

    mov ah, 0x0e ; printing the current char
    int 0x10

    add bx, 1
    jmp start

done:
    popa
    ret


print_nl:
    pusha

    mov ah, 0x0e
    mov al, 0x0a
    int 0x10

    mov al, 0x0d
    int 0x10

    popa
    ret
