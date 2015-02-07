int printf(const char* fmt, ...);
int puts(const char*);
int putchar(int);

/* sets the I/O functions, allows easy switching between text mode and VBE */
void set_putchar(void (*func)(char));
void set_puts(void (*func)(const char*));
