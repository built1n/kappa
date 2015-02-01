#include <stdint.h>
#include "panic.h"

#if UINT32_MAX == UINTPTR_MAX
#define STACK_CHK_GUARD 0xdeadbeef
#else
#define STACK_CHK_GUARD 0x0ddc0ffeebadf00d
#endif

uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

__attribute__((noreturn)) void __stack_chk_fail(void)
{
#if __STDC_HOSTED__
    abort();
#else
    panic("Stack smashing detected");
#endif
}
