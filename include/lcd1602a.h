#ifndef LCD1602A_H__
#define LCD1602A_H__

#include <stdint.h>

#define LCD_TOP_ROW 0
#define LCD_BOT_ROW 1

void lcd_init(void);
void lcd_println(char *str, uint8_t row);
void lcd_shift_down(void);

#endif
