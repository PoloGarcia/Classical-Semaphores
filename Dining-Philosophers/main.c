#include "semaforos.h"

int main(int argc, char* argv[]) {
	int semid, i;
	key_t key;
	
	if (argc != 1) {
		printf("forma de uso: %s", argv[0]);
		return -1;
	}
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("ftok");
		return -1;
	}
	
	if ( (semid = semget(key, 6, 0666 | IPC_CREAT))  < 0 ) {
		perror("semget");
		return -1;
	}
	
	for (i = 0; i < 5; ++i)
	{
		semctl(semid, i, SETVAL, 1);
	}

	semctl(semid, FOOTMAN, SETVAL, 4);

	return 0;
}