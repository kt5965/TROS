# Default target
all: prebuild bin/test.img

# Directories
SRC_DIR := src
BIN_DIR := bin
CC := gcc
LD := ld
OBJCOPY := objcopy

C_SOURCES := $(wildcard $(SRC_DIR)/*.c)
ASN_SOURCES := $(wildcard $(SRC_DIR)/*.asm)
C_OBJECTS := $(C_SOURCES:$(SRC_DIR)/%.c=$(SRC_DIR)/%.o)
ASM_OBJECTS := $(ASM_SOURCES:$(SRC_DIR)/%.asm=$(BIN_DIR)/%.img)

prebuild:
	@echo "C Objects: $(C_OBJECTS)"

# Assemble to produce a binary
$(BIN_DIR)/%.img: $(SRC_DIR)/%.asm
	nasm -f bin -I$(SRC_DIR) $< -o $@

# Compile finction.c
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -g -O0 -c -masm=intel -m32 -ffreestanding $< -o $@

# Link main.o
$(BIN_DIR)/main.img: $(SRC_DIR)/main.o $(SRC_DIR)/function.o $(SRC_DIR)/interrupt.o $(SRC_DIR)/shell.o $(SRC_DIR)/fs.o
	$(LD) -melf_i386 -Ttext 0x10200 -nostdlib $^ -o $@

$(BIN_DIR)/disk.img: $(BIN_DIR)/main.img
	$(OBJCOPY) -O binary $(BIN_DIR)/main.img $(BIN_DIR)/disk.img

bin/test.img: $(BIN_DIR)/boot1.img $(BIN_DIR)/kernel1.img $(BIN_DIR)/disk.img
	cat $(BIN_DIR)/boot1.img $(BIN_DIR)/kernel1.img $(BIN_DIR)/disk.img > $(BIN_DIR)/test.img

# clean
clean:
	rm -f $(BIN_DIR)/*.img $(SRC_DIR)/*.o

