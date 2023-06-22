
C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h)

OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o}

CFLAGS = -g -Wall -Wextra -fno-exceptions -m32


all: os-image

# Run qemu to simulate booting of our code.
run: all
	qemu-system-x86_64 -fda os-image -display curses

# Open the connection to qemu and load our kernel-object file with symbols
debug: os-image kernel.elf
	qemu-system-i386 -s -fda os-image &
	gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# This is the actual disk image that the computer loads,
# which is the combination of our compiled bootsector and kernel
os-image: boot/boot_sect.bin kernel.bin
	cat $^ > os-image

# This builds the binary of our kernel from two object files :
# - the kernel_entry , which jumps to main () in our kernel
# - the compiled C kernel
kernel.bin: kernel/kernel_entry.o ${OBJ}
	i386-elf-ld -T linker.ld -o kernel.bin $^ --oformat binary --verbose

# Used for debugging purposes
kernel.elf: kernel/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^

# Build our kernel object file .
%.o: %.c ${HEADERS}
	i386-elf-gcc ${CFLAGS} -masm=intel -ffreestanding -c $< -o $@

# Build our kernel entry object file .
%.o: %.asm
	nasm $< -f elf -o $@

# Assemble the boot sector to raw machine code
%.bin: %.asm
	nasm $< -f bin -I './boot/' -o $@

# Clear away all generated files .
clean:
	rm -fr *.elf *.bin *.dis *.o os-image *.map
	rm -fr kernel/*.o boot/*.bin drivers/*.o

# Disassemble our kernel - might be useful for debugging .
kernel.dis: kernel.bin
	ndisasm -b 32 $< > $@

