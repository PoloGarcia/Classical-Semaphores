#include "semaphores.h"

void penny_process() {
	int semid, element, time;
	key_t key;
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("penny ftok");
		exit(-1);
	}
	
	if ( (semid = semget(key, 4, 0666))  < 0 ) {
		perror("penny semget");
		exit(-1);
	}
	
	srand( getpid() );
	printf("Penny has started.\n");
	while (1) {
		mutex_wait(semid, TABLE);
		element = (rand() % 3) + 1;
		switch (element) {
			case BARRETTE: printf("Penny placed RHINOSTONE and FLOWER on the table.\n"); break;
			case RHINOSTONE: printf("Penny placed BARRETTE and FLOWER on the table.\n"); break;
			case FLOWER: printf("Penny placed RHINOSTONE and BARRETTE on the table.\n"); break;
		}
		mutex_signal(semid, element);
		time = (rand() % 5) + 5;
		printf("Penny goes to sleep %i seconds.\n", time);
		sleep(time);
	}
}

void leonard_process() {
	int semid, element, time;
	key_t key;
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("leonard ftok");
		exit(-1);
	}
	
	if ( (semid = semget(key, 4, 0666))  < 0 ) {
		perror("leonard semget");
		exit(-1);
	}
	
	srand( getpid() );
	printf("Leonard has started.\n");
	while (1) {
		mutex_wait(semid, BARRETTE);
		time = (rand() % 5) + 5;
		sleep(time);
		printf("Leonard takes RHINOSTONE and FLOWER and makes a Penny Blossoms.\n");
		mutex_signal(semid, TABLE);
	}
}

void sheldon_process() {
	int semid, element, time;
	key_t key;
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("sheldon ftok");
		exit(-1);
	}
	
	if ( (semid = semget(key, 4, 0666))  < 0 ) {
		perror("sheldon semget");
		exit(-1);
	}
	
	srand( getpid() );
	printf("Sheldon has started.\n");
	while (1) {
		mutex_wait(semid, RHINOSTONE);
		time = (rand() % 5) + 5;
		sleep(time);
		printf("Sheldon takes BARRETTE and FLOWER and makes a Penny Blossoms.\n");
		mutex_signal(semid, TABLE);
	}
}

void howard_process() {
	int semid, element, time;
	key_t key;
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("howard ftok");
		exit(-1);
	}
	
	if ( (semid = semget(key, 4, 0666))  < 0 ) {
		perror("howard semget");
		exit(-1);
	}
	
	srand( getpid() );
	printf("Howard has started.\n");
	while (1) {
		mutex_wait(semid, FLOWER);
		time = (rand() % 5) + 5;
		sleep(time);
		printf("Howard takes BARRETTE and RHINOSTONE and makes a Penny Blossoms.\n");
		mutex_signal(semid, TABLE);
	}
}

int main(int argc, char* argv[]) {
	key_t key;
	int pid, semid;
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("program ftok");
		return -1;
	}
	
	if ( (semid = semget(key, 4, 0666 | IPC_CREAT))  < 0 ) {
		perror("program semget");
		return -1;
	}
	
	semctl(semid, TABLE, SETVAL, 1);
	semctl(semid, BARRETTE, SETVAL, 0);
	semctl(semid, RHINOSTONE, SETVAL, 0);
	semctl(semid, FLOWER, SETVAL, 0);
	
	if ((pid = fork()) < 0) {
		perror("program fork 1");
		return -1;
	} else if (pid == 0) {
		penny_process();
	}
	
	if ((pid = fork()) < 0) {
		perror("program fork 2");
		return -1;
	} else if (pid == 0) {
		leonard_process();
	}
	
	if ((pid = fork()) < 0) {
		perror("program fork 3");
		return -1;
	} else if (pid == 0) {
		sheldon_process();
	}
	
	if ((pid = fork()) < 0) {
		perror("program fork 4");
		return -1;
	} else if (pid == 0) {
		howard_process();
	}
	
	sleep(10);
	
	semctl(semid, 0, IPC_RMID);
	
	return 0;
}
