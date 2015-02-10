#include <stdint.h>

void tty_init(void);
void tty_clear(void);
void tty_set_color(uint8_t);
uint8_t tty_get_color(void);
void tty_putchar_at(int ch, uint8_t color, int x, int y);
void tty_putchar(int ch);
void tty_puts(const char*);
