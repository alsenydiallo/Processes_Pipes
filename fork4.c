// demonstrates a child that terminates normally,
// and a parent that doesn't wait for the child,
// but that can still reap the child if it waits until the child terminates
#include <string.h> 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>         // for fork()
#include <sys/types.h>      // for pid_t
#include <sys/wait.h>       // for waitpid()

#define CHILD_WAIT      3

int main (int argc, char **argv)
{
    pid_t   pid;
    int     status;
    int     childRetVal = 0;
   // int     parentWait = 3;     // seconds

//	int mask = 0x0000FF00;
	char *str;
	int comm[2];

	if(pipe(comm)){
		perror("pipe");
		}

   // if (argc > 1)
        // get parent wait time from command line
       // parentWait = atoi(argv[1]);
   
   
    pid = fork();
    if (pid < 0) {
        printf("fork error %d\n", pid);
        return -1;
        }
    else if (pid == 0) {
        // code that runs in the child process
       // printf("child sleeps for %d sec\n", CHILD_WAIT);
       // sleep(CHILD_WAIT);      // seconds
       // printf("child returns %d\n", childRetVal);
      close(comm[1]);
	  read(comm[0], &str, sizeof(char*));
	  childRetVal = strlen(str);
	  
	  close(comm[0]);
	  return childRetVal;
        }
    else {
        // code that runs in the parent process
       // printf("parent sleeps for %d sec\n", parentWait);
       // sleep(parentWait);      // seconds
		close(comm[0]);

		write(comm[1], &argv[1], sizeof(char*));

        printf("parent calls waitpid\n");
        if (waitpid(pid, &status, 0) > 0) {
            printf("parent reaps child status 0x%08X\n", status);
            if (WIFEXITED(status))
                printf("child exited normally with %d\n", WEXITSTATUS(status));
            }
        printf("parent exits\n");
        return 0;
        }
}
