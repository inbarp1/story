#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>
#define KEYONE 010600
#define KEYTWO 666;
int main(int argc, char *argv[]){
  int sem_id, mem_id;
 if (argc==2) {
   if (!strcmp(argv[1],"-v")) {
     mem_id = shmget(KEYTWO,sizeof(char *), IPC_CREAT | IPC_EXCL | 0600);
     
     sem_id=semget(KEYONE,1,0600);
     printf("Semaphore value: %d\n",semctl(sem_id,0,GETVAL));
    }
    else if (!strcmp(argv[1],"-r")) {
      sem_id=semget(KEYONE,1,0600);
      mem_id=
      printf("Semaphore removed: %d\n",semctl(sem_id,0,IPC_RMID));
    }
    else
      printf("Improper input\n");
  }
 else if (argc==3) {
    if (!strcmp(argv[1],"-c")) {
      
     sem_id=semget(KEYONE,1,IPC_CREAT | IPC_EXCL | 0600);
     if (sem_id>=0) {
       int size=0;
       sscanf(argv[2],"%d",&size);
       //printf("Semaphore created: %d\n",sem_id);
       semctl(sem_id,0,SETVAL,size);
       //printf("Value set: %d\n", semctl(sem_id,0,GETVAL));
     }
     else {
       //printf("Semaphore already exists\n");
     }
   }
   else
      printf("improper input\n");
  }
  else
    printf("Improper input.\n");
  return 0;
 

}

