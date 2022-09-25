#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"
#include "kernel/fs.h"

int
main(int argc, char *argv[])
{
    sleep(5);
    //read from fist program (stdout turn into another program's stdin)
    char *xargv[MAXARG];
    int xargc = 0;
    int count = 0; //count first program stdin
    char buf[MAXARG];//read char array

    read(0, buf, MAXARG);
    //printf("buff is: %s, len is: %d\n", buf, strlen(buf));
    //catch xargs' argv
    for(int i=1; i<argc; ++i)
    {
        xargv[xargc] = argv[i];
        ++xargc;
    }
   
    char *p = buf;
    for(int i=0; i<sizeof(buf); ++i)
    {
        if(buf[i] == '\n')
        {
            count += 1;
            //printf("There is an enter line char!\n");
            if(fork() == 0)
            {
                buf[i] = 0; //strip '\n' char
                xargv[xargc] = p; //point to NULL
                ++xargc;
                xargv[xargc] = 0;
                ++xargc;
                // for(int i=0; i<(argc -1 + count); ++i)
                // {
                //     printf("xargv is : %s\n", xargv[i]);
                // }
                exec(xargv[0], xargv);
            }
            else
            {
                p = &buf[i + 1];
                wait((int *)0);

            }
        }

    }
    exit(0);
}