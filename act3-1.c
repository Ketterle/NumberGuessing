/*
*
* File act3-1.c
*   
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



void err_sys(const char* text) {
    perror(text);
    exit(1);
}

int main(int argc, char *argv[]) {
    int input;
    char buffer[10 + 2];
    sem_t* psem1;
    sem_t* psem2;
    int sem_value;
    char key;
    int count;
    int result;
    int shmid;
    int ident;
    
/* Creating structure */
struct shmseg {
        int sent;
        int guess;
        int iterations;
    };

/* Creating shared memory */
    shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0666|IPC_CREAT);
    if (shmid == -1) err_sys("Shared Memory Error");   
 
    struct shmseg *shmp;    
    shmp=shmat(shmid, NULL, 0);
    if (shmp == (void*)(-1)) err_sys("Shared Memory attachment error"); 
    
/* Parse program arguments */
    if (argc != 1) {
        fprintf(stderr, "Usage: ./act1-1\n");
        exit(1); 
    }
        
/* Create psem1 */
    psem1 = (sem_t*)sem_open("/semaphore1", O_CREAT, 0600, 0);
    if (psem1 == SEM_FAILED) {
        err_sys("Open psem1");
    }
    result = sem_getvalue(psem1, &sem_value);
    if (result < 0) {
        err_sys("Read psem1");
    }

/* Wait for sem_value to be 0 */
    while (sem_value > 0) {
        sem_wait(psem1);
        sem_value--;
    }
    
/* Repeat */
    count = 0;
    while (count < 1) {
    /* Increment the value of semaphore to initialize it to set argument */
        count++;
        if (sem_post(psem1) < 0) {
            err_sys("Post psem1");
        }
    }

   /* Read and print psem1 */
    result = sem_getvalue(psem1, &sem_value);
    if (result < 0) {
        err_sys("Read psem1");
    }
    printf("(SEM1): %d\n", sem_value);

/* Create psem2 */
    psem2 = (sem_t*)sem_open("/semaphore2", O_CREAT, 0600, 0);
    if (psem2 == SEM_FAILED) {
        err_sys("Open psem2");
    }
    result = sem_getvalue(psem2, &sem_value);
    if (result < 0) {
        err_sys("Read psem1");
    }

/* Wait for sem_value to be 0 */
    while (sem_value > 0) {
        sem_wait(psem2);
        sem_value--;
    }
    
/* Repeat */
    count = 0;
    while (count < 0) {
    /* Increment the value of semaphore to initialize it to set argument */
        count++;
        if (sem_post(psem2) < 0) {
            err_sys("Post psem2");
        }
    }

   /* Read and print psem2 */
    result = sem_getvalue(psem2, &sem_value);
    if (result < 0) {
        err_sys("Read psem2");
    }
    printf("(SEM2): %d\n", sem_value);
    
/* Close psem1 */
    result = sem_close(psem1);
    if (result != 0) {
        err_sys("Close psem1");
    }

/* Close psem2 */
    result = sem_close(psem2);
    if (result != 0) {
        err_sys("Close psem2");
    }
/* Printing global values */
    printf("Number of iterations: %d\n",shmp->iterations); 
}
  
         
   
