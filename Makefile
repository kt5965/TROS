# Makefile for boot2.asm and kernel1.asm

# Default target
all: bin/test.img

# Directories
SRC_DIR := src
BIN_DIR := bin

# Assemble boot1.asm to produce a binary
$(BIN_DIR)/boot1.bin: $(SRC_DIR)/boot1.asm
	nasm -f bin -I./src/ $(SRC_DIR)/boot1.asm -o $(BIN_DIR)/boot1.bin

# Assemble kernel1.asm to produce a binary
$(BIN_DIR)/kernel1.bin: $(SRC_DIR)/kernel1.asm
	nasm -f bin -I./src/ $(SRC_DIR)/kernel1.asm -o $(BIN_DIR)/kernel1.bin

# Create the bootable image
bin/test.img: $(BIN_DIR)/boot1.bin $(BIN_DIR)/kernel1.bin
	dd if=/dev/zero of=$(BIN_DIR)/test.img bs=512 count=2880
	dd if=$(BIN_DIR)/boot1.bin of=$(BIN_DIR)/test.img bs=512 conv=notrunc
	dd if=$(BIN_DIR)/kernel1.bin of=$(BIN_DIR)/test.img bs=512 seek=1 conv=notrunc
# clean
clean:
	rm -f $(BIN_DIR)/boot1.bin $(BIN_DIR)/kerbel1.bin test.img