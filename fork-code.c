#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

//base code is from the textbook, page 119
int main()
{
    pid_t pid;
    int count = 0;

    /* fork a child process */
    pid = fork();
    
    printf("pid: %d\n", pid);

    if (pid < 0) { /* error occurred */
        fprintf(stderr, "Fork Failed\n");
        return 1;
    }
    else if (pid == 0) {
        //count up infinitely
        while(1){ /* child process */
            count += 1;
            //inform that it is the child process
            printf("Cycle Number Child: %d\n", count);
            sleep(1);
        }
    }
    else { /* parent process */
        while(1){
        count += 1;
        //inform that it is the parent process
        printf("Cycle Number Parent: %d\n", count);
        sleep(1);
    }
    }


    return 0;
}
