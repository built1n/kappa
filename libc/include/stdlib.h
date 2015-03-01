#ifndef _STDLIB_H_
#define _STDLIB_H_

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

/* this is by no means standards-compliant... but who cares? :P */

/* NOT reentrant! */
char* itoa(int val, int base);

#define RAND_MAX ((1U << 31) - 1)

#define MIN(x,y) ((x<y)?x:y)
#define MAX(x,y) ((x>y)?x:y)

uint64_t rand64(void);
unsigned int rand(void);
void srand(uint64_t);
int abs(int);
void *malloc(size_t);
int snprintf(char*, int, const char*, ...);
void assert_fail(const char*, const char*, int);

#define assert(x) if(!(x))assert_fail(__func__, __FILE__, __LINE__);

#endif
