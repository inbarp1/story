#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <fcntl.h>
#define KEYONE 010600
#define KEYTWO 666
int main(int argc, char *argv[]){
  int sem_id, mem_id, fd;
  int * attach_mem;
   if (!strcmp(argv[1],"-v")) {
     sem_id=semget(KEYONE,1,0600);
     //printf("Semaphore value: %d\n",semctl(sem_id,0,GETVAL));
    }
    else if (!strcmp(argv[1],"-r")) {
      sem_id=semget(KEYONE,1,0600);
	//printf("Semaphore removed: %d\n",semctl(sem_id,0,IPC_RMID));
    } 
   else if (!strcmp(argv[1],"-c")) {
      sem_id=semget(KEYONE,1,IPC_CREAT | IPC_EXCL | 0600);
      if (sem_id>=0) {
	//printf("Semaphore created: %d\n",sem_id);
	semctl(sem_id,0,SETVAL,1);
	mem_id = shmget(KEYTWO, sizeof(char *), IPC_CREAT | IPC_EXCL | 0600);
	fd = open("story.txt", O_CREAT| O_TRUNC | O_RDWR, 0600);
	attach_mem = (int *)(shmat(sem_id,0, 0));
       //printf("Value set: %d\n", semctl(sem_id,0,GETVAL));
     }
     else {
       //printf("Semaphore already exists\n");
     }
   }
   else
     printf("improper input\n");
  return 0;
 

}

