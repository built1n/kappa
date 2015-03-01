#include <stdint.h>

void vgatext_init(void);
void vgatext_clear(void);
void vgatext_set_color(uint8_t);
uint8_t vgatext_get_color(void);
void vgatext_putchar_at(int ch, uint8_t color, int x, int y);
void vgatext_putchar(int ch);
void vgatext_puts(const char*);
