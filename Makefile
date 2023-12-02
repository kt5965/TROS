# Makefile for boot2.asm and kernel1.asm

# Default target
all: bin/test.img

# Directories
SRC_DIR := src
BIN_DIR := bin
CC := gcc
LD := ld
OBJCOPY := objcopy
# Assemble boot1.asm to produce a binary
$(BIN_DIR)/boot1.img: $(SRC_DIR)/boot1.asm
	nasm -f bin -I./src/ $(SRC_DIR)/boot1.asm -o $(BIN_DIR)/boot1.img

# Assemble kernel1.asm to produce a binary
$(BIN_DIR)/kernel1.img: $(SRC_DIR)/kernel1.asm
	nasm -f bin -I./src/ $(SRC_DIR)/kernel1.asm -o $(BIN_DIR)/kernel1.img

# Compile main.c
$(SRC_DIR)/main.o: $(SRC_DIR)/main.c
		$(CC) -c -m32 -ffreestanding $(SRC_DIR)/main.c -o $(SRC_DIR)/main.o

# Link main.o
$(BIN_DIR)/main.img: $(SRC_DIR)/main.o
		$(LD) -melf_i386 -Ttext 0x10200 -nostdlib  $(SRC_DIR)/main.o -o $(BIN_DIR)/main.img

$(BIN_DIR)/disk.img: $(BIN_DIR)/main.img
		$(OBJCOPY) -O binary $(BIN_DIR)/main.img $(BIN_DIR)/disk.img

# Create the bootable image
# bin/test.img: $(BIN_DIR)/boot1.bin $(BIN_DIR)/kernel1.bin $(BIN_DIR)/disk.img
# 		dd if=/dev/zero of=$(BIN_DIR)/test.img bs=512 count=2880
# 		dd if=$(BIN_DIR)/boot1.bin of=$(BIN_DIR)/test.img bs=512 conv=notrunc
# 		dd if=$(BIN_DIR)/kernel1.bin of=$(BIN_DIR)/test.img bs=512 seek=1 conv=notrunc
# 		dd if=$(BIN_DIR)/disk.img of=$(BIN_DIR)/test.img bs=512 seek=2 conv=notrunc
bin/test.img: $(BIN_DIR)/boot1.img $(BIN_DIR)/kernel1.img $(BIN_DIR)/disk.img
		cat $(BIN_DIR)/boot1.img $(BIN_DIR)/kernel1.img $(BIN_DIR)/disk.img > $(BIN_DIR)/test.img

# clean
clean:
		rm -f $(BIN_DIR)/boot1.img $(BIN_DIR)/kernel1.img $(SRC_DIR)/main.o $(BIN_DIR)/main.img $(BIN_DIR)/disk.img $(BIN_DIR)/test.img


# ...

