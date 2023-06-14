/*
*
* File act3-3.c
*   
*/

#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <termio.h>
#include <stdlib.h>
#include <stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/mman.h>
#include<time.h>
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
    bool found=false;
    int random;
    int iterations=0;
    srand((unsigned) time(NULL));    
    
/* Creating shared memory */

    struct shmseg {
        int sent;
        int guess;
        int iterations;
    };

    shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0666);
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
    psem1 = (sem_t*) sem_open("/semaphore1", O_CREAT, 0600, 0);
    if (psem1 == SEM_FAILED) {
        err_sys("Open psem1");
    }
 
/* Create psem2 */
    psem2 = (sem_t*) sem_open("/semaphore2", O_CREAT, 0600, 0);
    if (psem2 == SEM_FAILED) {
        err_sys("Open psem2");
    }

      
/* Generating random number */
    random=rand()%100;     
    
    while(true) {
        sem_wait(psem1);
        /* Entering critical zone */
        /*Number too big */
        if(random<shmp->guess) {
            shmp->sent=1;   
        }
        
        /*Number too small */
        else if(random>shmp->guess || shmp->iterations==0) {
            shmp->sent=-1;
             
        }
        
        /* Number matched */
        else {
            shmp->sent=0;
            printf("The random number is %d and client has found the right number in %d iterations  \n",random, shmp->iterations);
            sem_post(psem2);  
            break;
        }
        /* Getting out critical zone */
        sem_post(psem2);     
    }
             
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
}
