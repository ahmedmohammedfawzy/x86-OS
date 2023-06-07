print_hex:
    pusha
    mov cx, 0

hex_loop:
    cmp cx, 4 ; loop four times
    je end

    ; 1 convert the last char of dx into ascii
    mov ax, dx ; we use ax as working register
    and ax, 0x000f ; we mask out the first three bits, we later rotate the bits
    add al, 0x30 ; add 0x30 to convert hex character to its ascii representaion
    cmp al, 0x39 ; if the char is from "A" to "F" we add an extra 7 to convert to ascii
    jle step2
    add al, 7

step2:
    ; 2 get the postion in the hex string to place out ascii char
    mov bx, HEX_OUT + 5
    sub bx, cx
    mov [bx], al
    ror dx, 4

    add cx, 1
    jmp hex_loop

end:
    mov bx, HEX_OUT
    call print_string
    
    popa
    ret

HEX_OUT:
    db "0x0000", 0
