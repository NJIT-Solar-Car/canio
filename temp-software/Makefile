# Gemini generated makefile

# --- Configuration ---
MCU          = atmega328p
F_CPU        = 16000000UL
CC           = avr-gcc
OBJCOPY      = avr-objcopy
CFLAGS       = -Wall -g -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU)

# --- Directories & Files ---
TARGET       = main
BUILD_DIR    = build
SRCS         = main.c
OBJS         = $(addprefix $(BUILD_DIR)/, $(SRCS:.c=.o))

# --- Rules ---

# Default target: builds the hex file inside the build directory
all: $(BUILD_DIR)/$(TARGET).hex

# Link: Creates the .elf executable from object files
$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

# Copy: Translates the .elf file into standard Intel Hex format
$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@
	@echo "\n--- Build Successful! Hex file is at: $@ ---"

# Compile: Compiles each source file into an object file in the build directory
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Directory Creation: Ensures the build directory exists before compiling
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean: Wipes out the build directory and all generated files
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean