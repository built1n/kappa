#include <stdarg.h>

int vprintf(const char* fmt, va_list);
int printf(const char* fmt, ...)  __attribute__ ((format (printf, 1, 2)));
int puts(const char*);
int putchar(int);

/* sets the I/O functions, allows easy switching between text mode and VBE */
void set_putchar(void (*func)(int));
void set_puts(void (*func)(const char*));
