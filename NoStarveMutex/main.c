#include "semaforos.h"
#include <sys/shm.h>
#include <sys/sem.h>

int main(int argc, char *argv[])
{
	int i,semid, shmid;
	key_t key; 
	struct rooms *Rooms;

	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("ftok");
		return -1;
	}
	
	if ((semid = semget(key, 3, 0666 | IPC_CREAT))  < 0 ) {
		perror("semget");
		return -1;
	}

	semctl(semid, MUTEX, SETVAL, 1);
	semctl(semid, T1, SETVAL, 1);
	semctl(semid, T2, SETVAL, 0);

	if ( (shmid = shmget(key, sizeof(struct rooms), 0666 | IPC_CREAT)) < 0 ) {
		perror("shmid");
		return -1;
	}
	
	Rooms = shmat(shmid, NULL, 0);
	Rooms->room1 = 0;
	Rooms->room2 = 0;
	shmdt(Rooms);

	return 0;
}