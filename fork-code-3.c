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
    //run the child process
    else if (pid == 0) {
        while(1){ /* child process */
            count -= 1;
            //display multiples of 3
            if(count % 3 == 0){
                printf("Cycle Number Child: %d, %d is a multiple of 3\n", count, count);
            }

            else{
                printf("Cycle Number Child: %d\n", count);
            }
            //terminate the child when count reaches -500
            if(count == -500){
                printf("Child terminated\n");
                exit(1);
            }
    
            sleep(1);
        }
    }
    else { /* parent process */
        /* parent will wait for the child to complete */
       //run infinite loop
        while(1){
            //increase count
            count += 1;
            //print out if the cycle is a multiple of 3
            if(count % 3 == 0){
                printf("Cycle Number Parent: %d, %d is a multiple of 3\n", count, count);
            }
            else{
                printf("Cycle Number Parent: %d\n", count);
            }
            //waits for the child to terminate
            wait();
            printf("Parent terminated\n");
            exit(1);
            //wait for 1 second
            sleep(1);
    }
    }


    return 0;
}
