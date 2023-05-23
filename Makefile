# Thanks to cfenollosa (https://github.com/cfenollosa) for this amazing makefile
# (https://github.com/cfenollosa/os-tutorial/blob/master/13-kernel-barebones/Makefile)
# which dumb me could not make. (Hate you makefile syntax, but love you make)

C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c tasking/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h tasking/*.h)
# Nice syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o}

# Change this if your cross-compiler is somewhere else
CC = i386-elf-gcc
#GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb
# -g: Use debugging symbols in gcc
CFLAGS = -g -ffreestanding -Wall -Wextra -fno-exceptions -m32

# First rule is run by default
os-image.bin: boot/bootsect.bin kernel.bin
	cat $^ > os-image.bin

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
kernel.bin: ${OBJ}
	nasm kernel_entry.asm -f bin -o kernel_entry.bin && i386-elf-ld -o krnl.bin -Ttext 0x1000 $^ --oformat binary && cat kernel_entry.bin krnl.bin > $@

# Used for debugging purposes
kernel.elf: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0xC0000000 $^ 

run: os-image.bin
	/mnt/c/Program\ Files/Bochs-2.7/bochs.exe

# Open the connection to qemu and load our kernel-object file with symbols
#debug: os-image.bin kernel.elf
#	qemu-system-i386 -s -fda os-image.bin &
#	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# Generic rules for wildcards
# To make an object, always compile from its .c
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o os-image.bin *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o