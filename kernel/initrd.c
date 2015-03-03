#include <stddef.h>
#include <stdio.h>
#include "heap.h"
#include "multiboot.h"
#include "panic.h"

static struct multiboot_mod_t *modlist;
static unsigned int mods_count;

void initrd_read(void)
{
    if(modlist && mods_count)
    {
        printf("Number of modules: %d\n", mods_count);
        char *ptr = (char*)modlist[0].mod_start;
        while(ptr < (char*)modlist[0].mod_end)
        {
            char *name = ptr;
            ptr += 32;
            uint32_t sz = *(uint32_t*)ptr;
            ptr += 4;
            printf("File name: \"%s\"\n", name);
            printf("File length: %x\n", sz);
            char *end = ptr + sz;
            printf("Contents:\n");
            while(ptr < end)
            {
                putchar(*ptr++);
            }
        }
    }
    else
    {
        printf("No initrd loaded!\n");
    }
}

void initrd_init(struct multiboot_info_t *hdr)
{
    if(hdr->flags & (1<<3))
    {
        modlist = (struct multiboot_mod_t*)hdr->mods_addr;
        mods_count = hdr->mods_count;
        printf("Number of modules: %d\n", mods_count);
        /* prevent kmalloc from overwriting the modules */
        kmalloc_set_addr(modlist[mods_count-1].mod_end);
    }
    else
    {
        modlist = NULL;
        mods_count = 0;
    }
}
