#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <sys/types.h>
pid_t pid;
    
int shm_id;
//int shm_id_2;
int status;
//set up struct of variables that will share variables between processes
typedef struct{
    int count;
    int multiple;
}shared_data_t;

union semun {
    int val;                // Value for SETVAL
    struct semid_ds *buf;   // Buffer for IPC_STAT, IPC_SET
    unsigned short *array;  // Array for GETALL, SETALL
    struct seminfo *__buf;  // Buffer for IPC_INFO (Linux-specific)
};

int SemaphoreWait(int semid, int iMayBlock ) {
    struct sembuf sbOperation;
        sbOperation.sem_num = 0;
        sbOperation.sem_op = -1;
        sbOperation.sem_flg = iMayBlock;
    return semop( semid, &sbOperation, 1 );
}
int SemaphoreSignal( int semid ) {
    struct sembuf sbOperation;
        sbOperation.sem_num = 0;
        sbOperation.sem_op = +1;
        sbOperation.sem_flg = 0;
    return semop( semid, &sbOperation, 1 );
}
void SemaphoreRemove( int semid ) {
    if(semid != -1)
        semctl( semid, 0, IPC_RMID , 0);
    }
int SemaphoreCreate(int iInitialValue) {
    int semid;
    union semun suInitData;
    int iError;
    /* get a semaphore */
    semid = semget( IPC_PRIVATE, 1, IPC_CREAT);
    /* check for errors */
    if(semid == -1)
        printf("Here");
        return semid;
    /* now initialize the semaphore */
    suInitData.val = iInitialValue;
    if(semctl( semid, 0, SETVAL, suInitData) == -1)
    { /* error occurred, so remove semaphore */
        printf("Here1");
        SemaphoreRemove(semid);
        return -1;
    }
    return semid;
}
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

    int s_mutex;
    if((s_mutex = SemaphoreCreate(1)) == -1){
        SemaphoreRemove(s_mutex);
        perror("Error in SemaphoreCreate");
        exit(1);
    }
    //run the child process
    else if (pid == 0) {
        while(1){ /* child process */
            //only start when the count is greater than 200
            printf("----------------------------------------------\n");
            printf("Child Process: %d\n", pid);
            printf("%d : Waiting for Semaphore\n", pid);
            SemaphoreWait(s_mutex, 1);
            printf("%d : Semaphore aquired\n", pid);
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
            SemaphoreSignal(s_mutex);
            printf("%d : Semaphore released\n", pid);
            sleep(1);
        }
    }
    else { /* parent process */
        /* parent will wait for the child to complete */
       //run infinite loop
        while(1){
            //increase count
            printf("----------------------------------------------\n");
            printf("Parent Process: %d\n", pid);
            printf("%d : Waiting for Semaphore\n", pid);
            SemaphoreWait(s_mutex, 1);
            printf("%d : Semaphore aquired\n", pid);
            nums->count += 1;
            //print out if the cycle is a multiple of 3
            if(nums->count % (nums->multiple) == 0){
                printf("Cycle Number Parent: %d, %d is a multiple of %d\n", nums->count, nums->count, nums->multiple);
            }
            else{
                printf("Cycle Number Parent: %d\n", nums->count);
            }
            SemaphoreSignal(s_mutex);
            printf("%d : Semaphore released\n", pid);
            //wait for 1 second
            sleep(1);
    }
    }
    
    
    //detach memory from count
    shmdt(nums);

    return 0;
}
