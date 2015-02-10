#include <stdlib.h>

/* adapted from <http://www.strudel.org.uk/itoa/> */
char* itoa(int val, int base)
{
    static char buf[32] = {0};

    int neg = 0;

    if(val < 0)
    {
        val = -val;
        neg = 1;
    }

    int i = 30;

    do
    {
        buf[i] = "0123456789abcdef"[val % base];
        --i;
        val /= base;
    }
    while(val && i);

    if(neg)
    {
        buf[i] = '-';
    }

    return &buf[i+(neg?0:1)];
}

static unsigned long rand_s1 = 18, rand_s2 = 5, rand_s3 = 43;

#define M 1103515245UL
#define A 12345UL

/* this is a very fast tausworthe RNG */

unsigned int rand(void)
{
    rand_s1 = (((rand_s1&4294967294)<<12)^(((rand_s1<<13)^rand_s1)>>19));
    rand_s2 = (((rand_s2&4294967288)<< 4)^(((rand_s2<< 2)^rand_s2)>>25));
    rand_s3 = (((rand_s3&4294967280)<<17)^(((rand_s3<< 3)^rand_s3)>>11));
    return (rand_s1 ^ rand_s2 ^ rand_s3);
}

void srand(unsigned int seed)
{
    if(!seed)
        seed = 42;
    rand_s1 = seed++;
    rand_s2 = seed++;
    rand_s3 = seed++;
    /* "warm it up" */
    rand();
    rand();
    rand();
}

int abs(int val)
{
    return (val<0?-val:val);
}
