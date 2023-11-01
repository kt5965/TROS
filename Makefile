# Makefile for boot2.asm and kernel1.asm

# Default target
all: test.img

# Assemble boot2.asm to produce a binary
boot1.bin: boot1.asm
	nasm -f bin boot1.asm -o boot1.bin

# Assemble kernel1.asm to produce a binary
kernel1.bin: kernel1.asm
	nasm -f bin kernel1.asm -o kernel1.bin

# Create the bootable image
test.img: boot1.bin kernel1.bin
	dd if=/dev/zero of=test.img bs=512 count=2880
	dd if=boot1.bin of=test.img bs=512 conv=notrunc
	dd if=kernel1.bin of=test.img bs=512 seek=1 conv=notrunc

# Clean up the generated files
clean:
	rm -f boot1.bin kernel1.bin test.img

.PHONY: all clean
