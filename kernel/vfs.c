#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"
#include "vfs.h"

`static struct vfs_node *new_node(void)
{
    struct vfs_node *ret = kmalloc(sizeof(struct vfs_node));
    memset(ret, 0, sizeof(struct vfs_node));
    return ret;
}

struct vfs_node *vfs_init(void)
{
    struct vfs_node *root = new_node();
    root->flags = VFS_DIR;
    root->name = malloc(1);
    root->name[0] = '\0';

    return root;
}

int vfs_mkdir(struct vfs_node *node, const char *name)
{
    if(!node)
        return 0;
    if((node->flags & VFS_TYPEMASK) != VFS_DIR)
        return 0;
    /* insert a new element at the beginning of the linked list of the directory's children */
    struct vfs_node *newdir = new_node();
    newdir->flags = VFS_DIR;
    newdir->name = strdup(name);
    newdir->parent = node;
    newdir->next = node->child;

    node->child = newdir;
    return 1;
}

int vfs_creat(struct vfs_node *node, const char *name)
{
    if(!node)
        return 0;
    if((node->flags & VFS_TYPEMASK) != VFS_DIR)
        return 0;
    struct vfs_node *newfile = new_node();
    newfile->flags = VFS_REG;
    newfile->name = strdup(name);
    newfile->parent = node;
    newfile->next = node->child;

    node->child = newfile;
    return 1;
}

DIR *vfs_opendir(struct vfs_node *node)
{
    DIR *dirp = malloc(sizeof(DIR));
    dirp->node = node->child;
    return dirp;
}

struct dirent *vfs_readdir(DIR *dirp)
{
    if(!dirp->node)
        return NULL;
    static struct dirent ret;
    memcpy(ret.d_name, dirp->node->name, MIN(strlen(dirp->node->name)+1, sizeof(ret.d_name)));
    switch(dirp->node->flags & VFS_TYPEMASK)
    {
    case VFS_DIR:
        ret.d_type = DT_DIR;
        break;
    case VFS_REG:
        ret.d_type = DT_REG;
        break;
        /* fall through */
    case VFS_SPEC:
    default:
        ret.d_type = DT_UNKNOWN;
    }
    dirp->node = dirp->node->next;
    return &ret;
}

char *vfs_path(struct vfs_node *node)
{
    struct vfs_node *
}
