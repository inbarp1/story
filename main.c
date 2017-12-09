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

int main(){
  struct sembuf sb;
  int sem_key = ftok("control.c", 1);
  int shm_key = ftok("control.c", 2);
  int semid = semget(sem_key, 1, 0777);
  int shmid = shmget(shm_key, sizeof(int), 0777);
  int *shmp;

  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;
  printf("trying to access story...\n");
  semop(semid, &sb, 1);
  printf("accessed\n");

  shmp = (int *)shmat(shmid, 0, 0);
  char *buffer = (char *)calloc(256, sizeof(char));
  int fd = open("story.txt", O_RDWR);
  lseek(fd, -1 * *shmp, SEEK_END);
  read(fd, buffer, 256);
  printf("last line: %s\n", buffer);

  char *input = (char *)calloc(256, sizeof(char));
  printf("enter next line: \n");
  fgets(input, 256, stdin);
  *shmp = strlen(input);
  write(fd, input, *shmp);
  close(fd);

  int d = shmdt(shmp);
  if(d == -1){
    printf("shmdt error: %s", strerror(errno));
    exit(0);
  }

  sb.sem_op = 1;
  semop(semid, &sb, 1);
  printf("semaphore reset\n");
  return 0;
}
