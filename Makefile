
C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h)

OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o}

all: os-image

# Run qemu to simulate booting of our code.
run: all
	qemu-system-x86_64 -fda os-image -display curses

# This is the actual disk image that the computer loads,
# which is the combination of our compiled bootsector and kernel
os-image: boot/boot_sect.bin kernel.bin
	cat $^ > os-image

# This builds the binary of our kernel from two object files :
# - the kernel_entry , which jumps to main () in our kernel
# - the compiled C kernel
kernel.bin: kernel/kernel_entry.o ${OBJ}
	i386-elf-ld -o kernel.bin -Ttext 0x1000 $^ --oformat binary

# Build our kernel object file .
%.o: %.c ${HEADERS}
	i386-elf-gcc -Wall -Wextra -masm=intel -ffreestanding -c $< -o $@

# Build our kernel entry object file .
%.o: %.asm
	nasm $< -f elf -o $@

# Assemble the boot sector to raw machine code
%.bin: %.asm
	nasm $< -f bin -I './boot/' -o $@

# Clear away all generated files .
clean:
	rm -fr *.bin *.dis *.o os-image *.map
	rm -fr kernel/*.o boot/*.bin drivers/*.o

# Disassemble our kernel - might be useful for debugging .
kernel.dis: kernel.bin
	ndisasm -b 32 $< > $@

