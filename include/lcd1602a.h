#define LCD_TOP_ROW		0
#define LCD_BOT_ROW		1


void lcd_init(void);
void lcd_println(char *str, uint8_t len, uint8_t row);
void lcd_shift_down(void);
