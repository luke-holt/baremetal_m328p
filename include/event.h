
typedef void (*event_handler_t)(uint8_t);

void event_handler_register(uint8_t *flag_byte, event_handler_t handler);
void event_loop_start(void);
