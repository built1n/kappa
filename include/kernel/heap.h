#include <stddef.h>
#include <stdint.h>
void *kmalloc(size_t);
void *kmalloc_a(size_t);
void *kmalloc_p(size_t, void**);
void *kmalloc_ap(size_t, void**);
void kmalloc_set_addr(uint32_t);
