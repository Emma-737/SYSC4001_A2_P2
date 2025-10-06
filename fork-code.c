#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    pid_t pid;
    int count = 0;

    /* fork a child process */
    pid = fork();
    
    printf("pid: %d", pid);

    if (pid < 0) { /* error occurred */
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0) { /* child process */
        execlp("/bin/ls","ls",NULL);
    }
    else { /* parent process */
        /* parent will wait for the child to complete */
        wait(NULL);
        printf("Child Complete");
    }

    while(1){
        count += 1;
        printf("Cycle Number: %d", count);
    }


    return 0;
}
