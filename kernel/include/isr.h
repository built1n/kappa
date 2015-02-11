#include <stdint.h>

/* these are all implemented in isr-as.S */
extern void _isr0(void);
extern void _isr1(void);
extern void _isr2(void);
extern void _isr3(void);
extern void _isr4(void);
extern void _isr5(void);
extern void _isr6(void);
extern void _isr7(void);
extern void _isr8(void);
extern void _isr9(void);
extern void _isr10(void);
extern void _isr11(void);
extern void _isr12(void);
extern void _isr13(void);
extern void _isr14(void);
extern void _isr15(void);
extern void _isr16(void);
extern void _isr17(void);
extern void _isr18(void);
extern void _isr19(void);
extern void _isr20(void);
extern void _isr21(void);
extern void _isr22(void);
extern void _isr23(void);
extern void _isr24(void);
extern void _isr25(void);
extern void _isr26(void);
extern void _isr27(void);
extern void _isr28(void);
extern void _isr29(void);
extern void _isr30(void);
extern void _isr31(void);

/* installs ISR's 0-31 */
void isr_init(void);

/* This defines what the stack looks like after an ISR was running */
struct regs_t {
    uint32_t gs, fs, es, ds; /* pushed the segs last */
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    uint32_t int_no; /* interrupt stubs do this */
    uint32_t err_code; /* exceptions push this */
    uint32_t eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */
} __attribute__((packed));

#define IRQ(x) (32+x)

void set_interrupt_handler(uint8_t interrupt, void (*func)(struct regs_t*));
