#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

void write_le32(int fd, uint32_t n)
{
    write(fd, &n, 4);
}

#define MIN(x,y) (x<y?x:y)

int main(int argc, char* argv[])
{
    int fd = open("initrd.img", O_APPEND | O_WRONLY | O_CREAT, 0666);
    char buf[32];
    assert(strlen(argv[1]) < 32);
    memset(buf, 0, 32);
    memcpy(buf, argv[1], strlen(argv[1]));
    write(fd, buf, 32);
    int datafd = open(argv[1], O_RDONLY, 0666);
    lseek(datafd, 0, SEEK_END);
    FILE *f = fdopen(datafd, "rb");
    uint32_t len = ftell(f);
    write_le32(fd, ftell(f));
    int ch;
    lseek(datafd, 0, SEEK_SET);
    do {
        ch = getc(f);
        if(ch != EOF)
            write(fd, &ch, 1);
    } while(ch != EOF);
}
