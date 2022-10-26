PORT = /dev/ttyUSB1
FLASH_BAUD = 115200
MONITOR_BAUD = 9600
MCU = atmega328p

# 8 bits, no parity bit, 1 stop bit
USART_FORMAT = 8-N-1

# Directories
INC_DIRS = include
LIB_DIRS = $(INC_DIRS)
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
SRC_DIR = src

# Files
TARGET = $(BUILD_DIR)/app

SOURCES = \
	lcd1602a.c \
	event_msg.c \
	main.c

OBJECT_NAMES = $(SOURCES:.c=.o)
OBJECTS = $(addprefix $(OBJ_DIR)/,$(OBJECT_NAMES))

include hal/hal.mk

# Toolchain
CC = avr-gcc
OBJCP = avr-objcopy
AVRDUDE = sudo avrdude

# Flags
WFLAGS = -Wall -Wextra -Wshadow -Wpointer-arith \
		 -Wbad-function-cast -Wcast-align -Wsign-compare \
		 -Wstrict-prototypes \
		 -Wmissing-prototypes -Wmissing-declarations \
		 -Wunused
# Removed -Waggregate-return
COMMON_FLAGS = -Os -mmcu=$(MCU) -DF_CPU=16000000UL
CFLAGS = $(COMMON_FLAGS) $(WFLAGS) $(addprefix -I,$(INC_DIRS))
LDFLAGS = $(COMMON_FLAGS) $(addprefix -L,$(LIB_DIRS))


# Compiling
$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -c $^ -o $@
	@echo ""

# Linking. Remove eeprom section and convert binary to ihex format
$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $@)
	@echo "Linking..."
	@$(CC) $(LDFLAGS) -o $@ $^
	@echo "\nConverting to ihex format"
	@$(OBJCP) -O ihex -R .eeprom $@ $@.hex

# Phony targets
.PHONY: all clean flash monitor

# Default
all: $(TARGET)

clean:
	@rm -r $(BUILD_DIR)

flash: all
	@$(AVRDUDE) -F -V -c arduino -p $(MCU) -P $(PORT) -b $(FLASH_BAUD) -U flash:w:$(TARGET).hex

monitor:
	screen $(PORT) $(MONITOR_BAUD) $(USART_FORMAT)
