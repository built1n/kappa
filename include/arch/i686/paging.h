#define PAGE_PRESENT (1<<0)
#define PAGE_RW      (1<<1)
#define PAGE_USER    (1<<2)

#define PAGE_MASK 0xFFFFF000
#define PAGE_SIZE     0x1000

void paging_init(void);
