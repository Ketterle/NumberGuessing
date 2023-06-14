/*
 * Filename: act3-2.c
 */

#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <termio.h>
#include <stdlib.h>
#include <stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include <sys/mman.h>
#define STDINFD 0
#define SHM_KEY 0x1234

int main(int argc, char* argv[]) {

/* Creating structure */
 struct shmseg {
        int sent;
        int guess;
        int iterations;
    };

    int shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0666|IPC_CREAT);
    shmctl(shmid, IPC_RMID, NULL);

/* Check input arguments */
    if (argc != 1) {
        exit(1);
    }
    /* Dettaching shared memory */
     shmctl(SHM_KEY, IPC_RMID, NULL);
   
    /* Unlink semaphore 1 */
    sem_unlink("/semaphore1");
    /* Unlink semaphore 2 */
    sem_unlink("/semaphore2");
    
    printf("Semaphores and Shared Memory cleanup is done \n");

}
