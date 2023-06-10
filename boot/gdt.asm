gdt_start:

gdt_null: ; This is the 8 byte null descriptor
    dd 0x0 ; dd writes double word i.e 4 bytes
    dd 0x0 

gdt_code: ; code segment discriptor
    ; base=0x0 , limit=0xfffff
    ; access_byte_part1: ( present )1 ( privilege )00 ( descriptor type )1
    ; access_byte_part2: ( code )1 ( conforming )0 ( readable )1 ( accessed )0
    ; flags: ( granularity )1 (32 - bit default )1 (64 - bit seg )0 ( AVL )0
    dw 0xffff ; Limit bits (0-15)
    dw 0x0 ; Base bits (0-15)
    db 0x0 ; Base bits (16-23)
    db 10011010b ; access_byte
    db 11001111b ; flags, Limit bits (16-19)
    db 0x0 ; Base bits (24-31)

gdt_data: ; data segment discriptor
    ; same as the code segment discriptor except for access_byte_part2 
    ; access_byte_part2: ( code )0 ( expand down )0 ( writable )1 ( accessed )0
    dw 0xffff ; Limit bits (0-15)
    dw 0x0 ; Base bits (0-15)
    db 0x0 ; Base bits (16-23)
    db 10010010b ; access_byte
    db 11001111b ; flags, Limit bits (16-19)
    db 0x0 ; Base bits (24-31)

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

