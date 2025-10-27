#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>


//base code is from the textbook, page 119
int main()
{
    pid_t pid;
    //int *count; //shared memory
    //int *multiple; //shared memory
    int shm_id;
    int shm_id_2;
    int status;

    typedef struct{
        int count;
        int multiple;
    }shared_data_t;

    size_t shm_size1 = sizeof(shared_data_t);


    //set up shared memory
    //creates new flag with read and write permissions for the owner
    //if returns -1, there was an error
    if ((shm_id = (shmget (IPC_PRIVATE, shm_size1, IPC_CREAT | 0600))) == -1){
        perror("shmget error");
    }
    //for multiple
   
        
    if ( (shared_data_t *nums = (shared_data_t *) shmat (shm_id, (char *)0, 0 )) == (int *)-1){
        perror("shmat");
    }
    
    

    //*count = 0;
    //*multiple = 3;
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
            *count += 1;
            //display multiples of 3
            if(count % multiple == 0){
                printf("Cycle Number Child: %d, %d is a multiple of %d\n", shared_data_t.count, shared_data_t.count, shared_data_t.multiple);
            }

            else{
                printf("Cycle Number Child: %d\n", shared_data_t.count);
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
            if(*count % *multiple == 0){
                printf("Cycle Number Parent: %d, %d is a multiple of %d\n", count, count, multiple);
            }
            else{
                printf("Cycle Number Parent: %d\n", count);
            }
            
            sleep(1);
    }
    }
    //detach memory from count
    shmdt(nums);

    return 0;
}
