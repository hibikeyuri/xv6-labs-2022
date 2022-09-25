#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *
get_name(char *path)
{
    // Find first character after last slash
    char *p;
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++; // point to the next of slash address

    return p;
}

void
find(char *path, char *filename)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot fstat %s\n", path);
        return;
    }

    if (st.type != T_DIR)
    {
        fprintf(2, "must be a directory!\n");
        exit(1);
    }

    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf))
    {
        fprintf(2, "find: path too long!\n");
        exit(1);
    }

    strcpy(buf, path);
    //if path is ".", in the buf, data is "./"
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if (de.inum == 0)
            continue;
        //concate string
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        //printf("p is: %s, buf is: %s, get_name is: %s\n", p, buf, get_name(buf));
        if (stat(buf, &st) < 0)
        {
            printf("find: cannot stat %s\n", buf);
            continue;
        }

        if(strcmp(get_name(buf), filename) == 0)
        {
            printf("%s\n", buf);
        }

        if(st.type == T_DIR && strcmp(get_name(buf), ".") != 0 && strcmp(get_name(buf), "..") != 0)
        {
            //printf("enter directory!\n");
            find(buf, filename);
        }
        
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(2, "Usage: find <directory> <filename>\n");
        exit(1);
    }

    find(argv[1], argv[2]);

    exit(0);
}