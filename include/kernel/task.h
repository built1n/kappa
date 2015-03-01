#include <stdint.h>

struct taskregs_t {
    uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
};

struct task_t {
    struct taskregs_t regs;
    struct task_t *next;
};
