# ================================================================
# Makefile for STM32U575 Bare-Metal Project
# ================================================================

# Toolchain
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE = arm-none-eabi-size

# Project name
PROJECT = firmware

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Source files (from src directory)
SOURCES = $(wildcard $(SRC_DIR)/*.c)

# Object files (automatically generated from sources, placed in obj directory)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Compiler flags
CFLAGS = -mcpu=cortex-m33 \
         -mthumb \
         -Os \
         -ffreestanding \
         -nostdlib \
         -nostartfiles \
         -Wall \
         -Wextra \
         -g

# Linker flags
LDFLAGS = -mcpu=cortex-m33 \
          -mthumb \
          -nostdlib \
          -T linker.ld \
          -Wl,-Map=$(BIN_DIR)/$(PROJECT).map \
          -Wl,--gc-sections

# Default target
all: $(BIN_DIR)/$(PROJECT).bin $(BIN_DIR)/$(PROJECT).hex

# Create directories if they don't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Build the ELF file
$(BIN_DIR)/$(PROJECT).elf: $(OBJECTS) linker.ld | $(BIN_DIR)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	$(SIZE) $@

# Build the binary file (for flashing)
$(BIN_DIR)/$(PROJECT).bin: $(BIN_DIR)/$(PROJECT).elf
	$(OBJCOPY) -O binary $< $@

# Build the Intel HEX file (alternative format for flashing)
$(BIN_DIR)/$(PROJECT).hex: $(BIN_DIR)/$(PROJECT).elf
	$(OBJCOPY) -O ihex $< $@

# Compile C files to object files (in obj directory)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Show memory usage (requires .elf file)
size: $(BIN_DIR)/$(PROJECT).elf
	$(SIZE) $<

# Disassemble the binary (useful for debugging)
disasm: $(BIN_DIR)/$(PROJECT).elf
	$(OBJDUMP) -d $<

# Phony targets (not files)
.PHONY: all clean size disasm