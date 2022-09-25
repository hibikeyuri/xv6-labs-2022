#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void seive_by_prime(int left_reader, int right_writer, int prime)
{
    /*seive multiple of prime*/
    int n;

    while (read(left_reader, &n, sizeof(n)) == sizeof(n))
    {
        if (n % prime != 0)
        {
            write(right_writer, &n, sizeof(n));
        }
    }

    close(left_reader);
    close(right_writer);
}

void make_prime_process(int left_reader)
{
    int prime;
    if (read(left_reader, &prime, sizeof(prime)) == sizeof(prime))
    {
        printf("prime %d\n", prime);

        int seive_pipe[2];
        pipe(seive_pipe);

        seive_by_prime(left_reader, seive_pipe[1], prime);

        if(fork() == 0)
        {
            close(seive_pipe[0]);
            seive_by_prime(left_reader, seive_pipe[1], prime);
            close(left_reader);
            close(seive_pipe[1]);
            wait((int *)0);

        }
        else
        {
            close(left_reader);
            close(seive_pipe[1]);
            make_prime_process(seive_pipe[0]);
            close(seive_pipe[0]);
            exit(0);

        }

        exit(0);
    }
    else
    {
        close(left_reader);
        exit(1);
    }
}

void make_empty_process()
{
    int pd[2];
    pipe(pd);

    if (fork() == 0)
    {
        printf("child created!\n");
    }
    else
    {
        wait((int *)0);
    }
    close(pd[1]);
    close(pd[2]);
}

int main(int argc, char *argv[])
{
    int main_pipe[2];
    pipe(main_pipe);

    for (int i = 2; i <= 35; ++i)
    {
        write(main_pipe[1], &i, sizeof(i));
    }

    if (fork() == 0)
    {
        close(main_pipe[1]);
        make_prime_process(main_pipe[0]);
        // make_empty_process();
    }
    else
    {
        close(main_pipe[1]);
        close(main_pipe[0]);
        wait((int *)0);
    }

    exit(0);
}