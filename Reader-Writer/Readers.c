#include "semaforos.h"
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>

void reader(){
	int semid, shmid;
	key_t key;
	struct readers *Readers;

	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("ftok");
		exit(-1);
	}

	if ( (semid = semget(key, 3, 0666))  < 0 ) {
		perror("semget");
		exit(-1);
	}

	if ( (shmid = shmget(key, sizeof(struct readers), 0666)) < 0 ) {
		perror("shmid");
		exit(-1);
	}

	Readers = shmat(shmid, NULL, 0);

	while(1) {
		
		/*
		mutex.wait()
		    readers += 1
		    if readers == 1:
		       roomEmpty.wait()
		mutex.signal()

		# first in locks

		# critical section for readers
		mutex.wait()
		    readers -= 1
		    if readers == 0:
		       roomEmpty.signal() # last out unlocks
		mutex.signal() 
		*/
		mutex_wait(semid,MUTEX);
			Readers->qty = Readers->qty +1;
			if (Readers->qty == 1)
			{
				printf("Waiting for empty room\n");
				mutex_wait(semid,ROOM_EMPTY);
			}
		mutex_signal(semid,MUTEX);

		printf("Critical section for reader: %i\n",getpid());
		sleep(3);

		mutex_wait(semid,MUTEX);
			Readers->qty = Readers->qty - 1;
			if (Readers->qty == 0)
			{
				printf("Now the room is empty\n");
				mutex_signal(semid,ROOM_EMPTY);
			}
		mutex_signal(semid,MUTEX);
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
			reader();
		} else {
		}
	}
	return 0;
}