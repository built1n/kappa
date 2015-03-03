#include <stdint.h>

#define VFS_DIR 0
#define VFS_REG 1
#define VFS_SPEC 2

#define VFS_TYPEMASK 0x3

struct vfs_node {
    unsigned int flags;
    char *name;
    uint64_t file_len;
    unsigned char *contents;
    struct vfs_node *parent;
    struct vfs_node *next;
    struct vfs_node *child;
};

#define DT_BLK (1<<0)
#define DT_CHR (1<<1)
#define DT_DIR (1<<2)
#define DT_FIFO (1<<3)
#define DT_LNK (1<<4)
#define DT_REG (1<<5)
#define DT_SOCK (1<<6)
#define DT_UNKNOWN (1<<7)

struct dirent {
    unsigned char d_type;
    char d_name[256];
};

/* initializes an empty filesystem on the heap */
struct vfs_node *vfs_init(void);

/* creates a new, empty directory UNDER a directory node */
int vfs_mkdir(struct vfs_node*, const char *name);

/* creates a new, empty, regular file under a directory node */
int vfs_creat(struct vfs_node*, const char *name);

typedef struct DIR {
    struct vfs_node *node;
} DIR;

DIR *vfs_opendir(struct vfs_node*);

struct dirent *vfs_readdir(DIR *dirp);
