#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

pid_t pid;
    
    int shm_id;
    //int shm_id_2;
    int status;
    //set up struct of variables that will share variables between processes
    typedef struct{
        int count;
        int multiple;
    }shared_data_t;

//base code is from the textbook, page 119
int main()
{    
    shared_data_t * nums;
    
    
    //find the size of the nums data struct
    int shm_size1 = sizeof(*nums);
   

    //set up shared memory
    //creates new flag with read and write permissions for the owner
    //if returns -1, there was an error
    if ((shm_id = (shmget (IPC_PRIVATE, shm_size1, IPC_CREAT | 0600))) == -1){
        perror("shmget error");
    }
    
   //assign shared memory to data struct
    nums = (shared_data_t *)shmat(shm_id, NULL, 0 );
    if (nums == (void *)-1){
        perror("shmat");
    }

    nums-> multiple = 3;
    
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
            //only start when the count is greater than 200
            if(nums->count > 20){
                nums->count += 1;
                //display multiples of 3
                if(nums->count % (nums->multiple) == 0){
                    printf("Cycle Number Child: %d, %d is a multiple of %d\n", nums->count, nums->count, nums->multiple);
                }

                else{
                    printf("Cycle Number Child: %d\n", nums->count);
                }
            }
    
            sleep(1);
        }
    }
    else { /* parent process */
        /* parent will wait for the child to complete */
       //run infinite loop
        while(1){
            //increase count
            nums->count += 1;
            //print out if the cycle is a multiple of 3
            if(nums->count % (nums->multiple) == 0){
                printf("Cycle Number Parent: %d, %d is a multiple of %d\n", nums->count, nums->count, nums->multiple);
            }
            else{
                printf("Cycle Number Parent: %d\n", nums->count);
            }
           
            //wait for 1 second
            sleep(1);
    }
    }
    
    
    //detach memory from count
    shmdt(nums);

    return 0;
}
