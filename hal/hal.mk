INC_DIRS += hal
HAL_OBJ_DIR = $(OBJ_DIR)/hal
HAL_SRC_DIR = hal/src

HAL_SOURCES = \
	gpio.c \
	usart.c \
	adc.c \
	pwm.c \
	spi.c

HAL_OBJECT_NAMES = $(HAL_SOURCES:.c=.o)

OBJECTS += $(addprefix $(HAL_OBJ_DIR)/,$(HAL_OBJECT_NAMES))

# Rule for compiling hal sources
$(HAL_OBJ_DIR)%.o: $(HAL_SRC_DIR)%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -c $^ -o $@
	@echo ""
