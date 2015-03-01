#include "task.h"

void task_create(struct task_t *task, void (*func)(void), uint32_t flags, uint32_t *pagedir)
{
    task->regs.eax = 0;
    task->regs.ebx = 0;
    task->regs.ecx = 0;
    task->regs.edx = 0;
    task->regs.esi = 0;
    task->regs.edi = 0;
    task->regs.eflags = flags;
    task->regs.eip = (uint32_t)main;
    task->regs.cr3 = (uint32_t)pagedir;
    task->regs.esp = kmalloc(0x1000) + 0x1000;
    task->next = NULL;
}
