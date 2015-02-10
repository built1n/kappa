/* this is by no means standards-compliant... but who cares? :P */

/* NOT reentrant! */
char* itoa(int val, int base);

#define RAND_MAX ((1U << 31) - 1)

#define MIN(x,y) ((x<y)?x:y)

unsigned int rand(void);
void srand(unsigned int);
int abs(int);
