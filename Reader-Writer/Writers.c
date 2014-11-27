#include "semaforos.h"
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>

void writer(){
	int semid, shmid;
	key_t key;
	struct readers *Readers;

	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("ftok");
		exit(-1);
	}

	if ( (semid = semget(key, 2, 0666))  < 0 ) {
		perror("semget");
		exit(-1);
	}

	if ( (shmid = shmget(key, sizeof(struct readers), 0666)) < 0 ) {
		perror("shmid");
		exit(-1);
	}

	Readers = shmat(shmid, NULL, 0);

	while(1) {

		/*roomEmpty.wait()*/
		mutex_wait(semid,ROOM_EMPTY);
			/*critical section for writers*/
			printf("%i ESTA ESCRIBIENDO\n",getpid());
			sleep(6);
		/*roomEmpty.signal()*/
		mutex_signal(semid,ROOM_EMPTY);
	}
	shmdt(Readers);
}

int main(int argc, char *argv[])
{	
	int pid,i;
	if (argc != 1)
	{
		printf("forma de uso: %s\n",argv[0]);
	}

	for (i = 0; i < 3; ++i)
	{
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			return -1;
		} else if (pid == 0) {
			writer();
		} else {
		}
	}
	return 0;
}