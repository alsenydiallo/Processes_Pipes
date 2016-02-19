// demonstrates multiple children, under control

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>         // for fork()
#include <sys/types.h>      // for pid_t
#include <sys/wait.h>       // for waitpid()
#include <signal.h>         // for kill()

#define     N_TICK_PROCESSES    3

typedef struct {
    char    *ch;            // character to print
    char    *interval;      // seconds
    pid_t   pid;
    }
TickSpecifier;

TickSpecifier   ts[N_TICK_PROCESSES] = {{"+", "1", 0},
                                        {"-", "2", 0},
                                        {"!", "3", 0}
                                        };

int main (int argc, char **argv)
{
    char    line[128];
    pid_t   pid;
    int     i;

    for (i = 0; i < N_TICK_PROCESSES; i++) {
        pid = fork();
        if (pid == 0)
            execl("tick", "tick", ts[i].ch, ts[i].interval, (char *) 0);
                // execl never returns
                // the first argument would be "tick.exe" in Cygwin
        else
            ts[i].pid = pid;    // might be < 0 if fork() error
        }

    gets(line);                 // wait for user to type a return

    // kill the children
    for (i = 0; i < N_TICK_PROCESSES; i++) {
        pid = ts[i].pid;
        if (pid > 0)
            kill(pid, SIGKILL);
        }

    return 0;
}
