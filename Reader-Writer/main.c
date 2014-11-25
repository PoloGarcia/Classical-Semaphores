#include "semaforos.h"
#include <sys/shm.h>
#include <sys/sem.h>

int main(int argc, char *argv[])
{
	int i,semid, shmid;
	key_t key; 
	struct readers *Readers;

	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("ftok");
		return -1;
	}
	
	if ((semid = semget(key, 2, 0666 | IPC_CREAT))  < 0 ) {
		perror("semget");
		return -1;
	}

	semctl(semid, MUTEX, SETVAL, 1);
	semctl(semid, ROOM_EMPTY, SETVAL, 1);

	if ( (shmid = shmget(key, sizeof(struct readers), 0666 | IPC_CREAT)) < 0 ) {
		perror("shmid");
		return -1;
	}
	
	Readers = shmat(shmid, NULL, 0);
	Readers->qty = 0;
	shmdt(Readers);

	return 0;
}