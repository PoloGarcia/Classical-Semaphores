#include "semaforos.h"
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>
/*

*/

void smoker_tobacco(){
	int semid, shmid;
	key_t key;
	struct table *Table;

	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("ftok");
		exit(-1);
	}

	if ( (semid = semget(key, 8, 0666))  < 0 ) {
		perror("semget");
		exit(-1);
	}

	if ( (shmid = shmget(key, sizeof(struct table), 0666)) < 0 ) {
		perror("shmid");
		exit(-1);
	}

	Table = shmat(shmid, NULL, 0);

	while(1) {
		sem_wait(semid,TOBACCO_SEM,1);
		printf("Smoker with TOBACCO making cigarrette\n");
		sleep(3);
		sem_signal(semid,AGENT,1);
	}

	shmdt(Table);
	exit(0);
}

void smoker_paper(){
	int semid, shmid;
	key_t key;
	struct table *Table;

	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("ftok");
		exit(-1);
	}

	if ( (semid = semget(key, 8, 0666))  < 0 ) {
		perror("semget");
		exit(-1);
	}

	if ( (shmid = shmget(key, sizeof(struct table), 0666)) < 0 ) {
		perror("shmid");
		exit(-1);
	}

	Table = shmat(shmid, NULL, 0);

	while(1) {
		sem_wait(semid,PAPER_SEM,1);
		printf("Smoker with PAPER making cigarrette\n");
		sleep(3);
		sem_signal(semid,AGENT,1);
	}

	shmdt(Table);
	exit(0);
}

void smoker_matches(){
	int semid, shmid;
	key_t key;
	struct table *Table;

	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("ftok");
		exit(-1);
	}

	if ( (semid = semget(key, 8, 0666))  < 0 ) {
		perror("semget");
		exit(-1);
	}

	if ( (shmid = shmget(key, sizeof(struct table), 0666)) < 0 ) {
		perror("shmid");
		exit(-1);
	}

	Table = shmat(shmid, NULL, 0);

	while(1) {
		sem_wait(semid,MATCHES_SEM,1);
		printf("Smoker with MATCHES making cigarrette\n");
		sleep(3);
		sem_signal(semid,AGENT,1);
	}

	shmdt(Table);
	exit(0);
}

int main(int argc, char *argv[])
{	
	int pid,i;
	if (argc != 1)
	{
		printf("forma de uso: %s\n",argv[0]);
	}

	if ( (pid = fork()) < 0 ) {
		perror("fork1");
		return -1;
	} else if (pid == 0) {
		printf("SMOKER WITH TOBACCO ...\n");
		smoker_tobacco();
	}

	if ( (pid = fork()) < 0 ) {
		perror("fork2");
		return -1;
	} else if (pid == 0) {
		printf("SMOKER WITH MATCHES ... \n");
		smoker_matches();
	}

	if ( (pid = fork()) < 0 ) {
		perror("fork3");
		return -1;
	} else if (pid == 0) {
		printf("SMOKER WITH PAPER ... \n");
		smoker_paper();
	}

	return 0;
}