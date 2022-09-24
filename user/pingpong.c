#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int fd_parent_to_child[2];
    int fd_child_to_parent[2];

    pipe(fd_child_to_parent);
    pipe(fd_parent_to_child);

    int pid = fork();
    int status = 0;

    char buf_char = 'A';

    if(pid < 0)
    {
        fprintf(2, "pid < 0, error!\n");
        close(fd_child_to_parent[0]);
        close(fd_child_to_parent[1]);
        close(fd_parent_to_child[0]);
        close(fd_parent_to_child[1]);
        status = 1;
    }
    else if (pid == 0)
    {
        /*read one character from parent process*/
        if(read(fd_parent_to_child[0], &buf_char, sizeof(buf_char)) != sizeof(buf_char))
        {
            printf("read from parent error!\n");
            status = 1;
        }
        else
        {
            printf("%d: received ping\n", getpid());
            status = 0;
        }

        if(write(fd_child_to_parent[1], &buf_char, sizeof(buf_char)) != sizeof(buf_char))
        {
            printf("write to parent error!\n");
            status = 1;
        }
        else
        {
            status = 0;
        }
    }
    else
    {
         /*read one character from parent process*/
        if(write(fd_parent_to_child[1], &buf_char, sizeof(buf_char)) != sizeof(buf_char))
        {
            printf("write to child error!\n");
            status = 1;
        }
        else
        {
            status = 0;
        }

        if(read(fd_child_to_parent[0], &buf_char, sizeof(buf_char)) != sizeof(buf_char))
        {
            printf("read from child error!\n");
            status = 1;
        }
        else
        {
            printf("%d: received pong\n", getpid());
            status = 0;
        }       
    }

    exit(status);
}