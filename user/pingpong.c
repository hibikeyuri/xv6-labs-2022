#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int fd_parent_to_child[2];
    int fd_child_to_parent[2];

    pipe(&fd_child_to_parent);
    pipe(&fd_parent_to_child);

    int pid = fork();
    int status = 0;

    char buf_char = "a";

    if(pid < 0)
    {
        fprintf(2, "something error happend!\n");
        close(fd_child_to_parent[0]);
        close(fd_child_to_parent[1]);
        close(fd_parent_to_child[0]);
        close(fd_parent_to_child[1]);
        exit(1);
    }
    else if (pid == 0)
    {
        
    }
    else
    {

    }
    
}