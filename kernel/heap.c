#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "paging.h"

/* a simple pointer-arithmetic-based malloc */

/* a symbol put there by the linker */
extern uint32_t link_mem_end;
uint32_t kmalloc_addr = &link_mem_end;

static void *kmalloc_int(size_t sz, int align, void **phys)
{
    if(align)
    {
        kmalloc_addr += (PAGE_SIZE - 1);
        kmalloc_addr &= PAGE_MASK;
    }
    if(phys)
        *phys = (void*)kmalloc_addr;
    void *ret = (void*)kmalloc_addr;
    kmalloc_addr += sz;
    assert(((uint32_t)ret & 0xFFF) == 0);
    return ret;
}

void *kmalloc(size_t sz)
{
    return kmalloc_int(sz, 0, NULL);
}

void *kmalloc_a(size_t sz)
{
    return kmalloc_int(sz, 1, NULL);
}

void *kmalloc_p(size_t sz, void **phys)
{
     return kmalloc_int(sz, 0, phys);
}

void *kmalloc_ap(size_t sz, void **phys)
{
    return kmalloc_int(sz, 1, phys);
}
