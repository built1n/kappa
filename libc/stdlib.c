#include <stdlib.h>

/* adapted from <http://www.strudel.org.uk/itoa/> */
char* itoa(int val, int base)
{
    static char buf[32] = {0};

    int i = 30;

    for(; val && i ; --i, val /= base)

        buf[i] = "0123456789abcdef"[val % base];

    return &buf[i+1];
}

static int rand_state = 42;

/* some constants for the RNG */
#define A 48271
#define M 2147483647
#define Q (M/A)
#define R (M%A)

int rand(void)
{
    int tmp = A * (rand_state % Q) - R * (rand_state / Q);
    if(tmp >= 0)
        rand_state = tmp;
    else
        rand_state = tmp + M;
    return rand_state;
}

void srand(unsigned int seed)
{
    rand_state = seed;
}
