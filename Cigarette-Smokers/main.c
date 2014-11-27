#include "semaforos.h"
#include <sys/shm.h>
#include <sys/sem.h>

int main(int argc, char *argv[])
{
	int i,semid, shmid;
	key_t key; 
	struct table *Table;

	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("ftok");
		return -1;
	}
	
	if ((semid = semget(key, 8, 0666 | IPC_CREAT))  < 0 ) {
		perror("semget");
		return -1;
	}

	semctl(semid, AGENT, SETVAL, 1);
	semctl(semid, MUTEX, SETVAL, 1);
	semctl(semid, TOBACCO, SETVAL, 0);
	semctl(semid, TOBACCO_SEM, SETVAL, 0);
	semctl(semid, PAPER, SETVAL, 0);
	semctl(semid, PAPER_SEM, SETVAL, 0);
	semctl(semid, MATCHES, SETVAL, 0);
	semctl(semid, MATCHES_SEM, SETVAL, 0);


	if ( (shmid = shmget(key, sizeof(struct table), 0666 | IPC_CREAT)) < 0 ) {
		perror("shmid");
		return -1;
	}
	
	Table = shmat(shmid, NULL, 0);
	Table->isTobacco = 0;
	Table->isPaper = 0;
	Table->isMatches = 0;
	shmdt(Table);

	return 0;
}