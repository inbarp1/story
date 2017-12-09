#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <unistd.h>

union semun {
  int val;
  struct semid_ds *buf;
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO
                              (Linux-specific) */
} arg;

void print_story(){
  int fork_id = fork();
  if(fork_id){
    int status;
    wait(&status);
  }
  else{
    char *args[] = {"cat", "story.txt", 0};
    execvp(args[0], args);
  }
}

int main(int argc, char *argv[]){
  struct sembuf sb;
  int sem_key = ftok("control.c", 1);
  int shm_key = ftok("control.c", 2);
  int semid, shmid, fd;
  
  if(argc == 1){
    printf("missing flags\n");
  }
  else if(!strncmp(argv[1], "-c", 2)){
    semid = semget(sem_key, 1, IPC_CREAT | IPC_EXCL | 0777);
    if(semid == -1){
      printf("sem error: %s\n", strerror(errno));
      exit(0);
    }

    shmid = shmget(shm_key, sizeof(int), IPC_CREAT | IPC_EXCL | 0777);
    if(shmid == -1){
      printf("shm error: %s\n", strerror(errno));
      exit(0);
    }

    fd = open("story.txt", O_CREAT | O_EXCL | O_TRUNC | O_RDWR, 0777);
    if(fd == -1){
      printf("file error: %s\n", strerror(errno));
      exit(0);
    }
    close(fd);

    if(semid != -1 && shmid != -1 && fd != -1){
      arg.val = 1;
      semctl(semid, 0, SETVAL, arg);
      //printf("semaphore %d created with value %d\n", semid, 1);
      //printf("shared memory %d created\n", shmid);

      int *shmp;
      shmp = (int *)shmat(shmid, 0, 0);
      if(shmp == -1){
        printf("shmat error: %s", strerror(errno));
        exit(0);
      }
      *shmp = 0;

      int d = shmdt(shmp);
      if(d == -1){
        printf("shmdt error: %s", strerror(errno));
        exit(0);
      }
      printf("story created\n");
    }
  }
  else if(!strncmp(argv[1], "-v", 2)){
    printf("full current story:\n");
    print_story();
    //semid = semget(sem_key, 1, 0777);
    //if(semid == -1) printf("error: %s\n", strerror(errno));
    //else{
    //printf("semaphore %d value: %d\n", semid, semctl(semid, 0, GETVAL, arg));
    //}
  }
  else if(!strncmp(argv[1], "-r", 2)){
    semid = semget(sem_key, 1, 0777);
    if(semid == -1) printf("error: %s\n", strerror(errno));
    shmid = shmget(shm_key, sizeof(int), 0777);
    if(shmid == -1) printf("error: %s\n", strerror(errno));

    if(semid != -1 && shmid != -1){
      sb.sem_num = 0;
      sb.sem_flg = SEM_UNDO;
      sb.sem_op = -1;
      printf("trying to access story...\n");
      semop(semid, &sb, 1);
      printf("accessed\n");

      printf("current story:\n");
      print_story();
      semctl(semid, 0, IPC_RMID, arg);
      //printf("semaphore %d removed\n", semid);
      shmctl(shmid, IPC_RMID, NULL);
      //printf("shared memory %d removed\n", shmid);
      remove("story.txt");
      printf("story removed\n");
    }
  }
  else{
    printf("invalid flags\n");
  }

  return 0;
}
