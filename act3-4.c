/*
*
* File act3-4.c
*   
*/

#include <semaphore.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <termio.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <time.h>
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
    int random;
    int z=50;
    int z1=0;
    int z2=100;
    /*srand((unsigned) time(NULL));*/
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


/* Insert numbers to the matrix */    
    while(true) {
        sem_wait(psem2);
        /* Entering critical zone */
        if(shmp->iterations==0) {
            z=50;
        }
/* Number too small */          
        if(shmp->sent==-1){
            if(shmp->iterations==0) {
                z=0;
                z2=100;    
            }
            printf("Selected number in the range [%d , %d]: ", z,z2);
            z1=z;
            z=z1+(z2-z1)/2;
            printf("%d \n", z);
            shmp->guess=z; 
            shmp->iterations++;
        }
/*Number too big*/ 
       else if(shmp->sent==1){
         if(z==1){
            printf("Selected number in the range [%d , %d]: ", z,z2);
            z=0;
            printf("%d \n", z);
        }

        else {
            if(shmp->iterations==0){
                z2=100;
                printf("Selected number in the range [%d , %d]: ", z1,z2);
            }
            else {
                 printf("Selected number in the range [%d , %d]: ", z1,z);
                z2=z;
            }
            z=z2-(z2-z1)/2;
            printf("%d \n",z);
        }
        shmp->guess=z;
        shmp->iterations++;
    }
    
    else {
        sem_post(psem1);
        break;
    }
    /* Getting out critical zone */
    sem_post(psem1);     
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
    printf("\nGuess number is: %d\nNumber of iterations: %d\n",shmp->guess,shmp->iterations); 
}




