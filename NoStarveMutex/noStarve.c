#include "semaforos.h"
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>
/*
mutex.wait()
     room1 += 1
 mutex.signal()
 t1.wait()
     room2 += 1
     mutex.wait()
     room1 -= 1
     if room1 == 0:
        mutex.signal()
        t2.signal()
     else:
        mutex.signal()
        t1.signal()
 t2.wait()
     room2 -= 1
     # critical section
     if room2 == 0:
        t1.signal()
     else:
        t2.signal()
*/

void cliente(pelo_crece){
	int semid, shmid;
	key_t key;
	struct rooms *Rooms;

	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("ftok");
		exit(-1);
	}

	if ( (semid = semget(key, 3, 0666))  < 0 ) {
		perror("semget");
		exit(-1);
	}

	if ( (shmid = shmget(key, sizeof(struct rooms), 0666)) < 0 ) {
		perror("shmid");
		exit(-1);
	}

	Rooms = shmat(shmid, NULL, 0);

	while(1) {
		
		mutex_wait(semid,MUTEX);
			Rooms->room1 += 1;
			printf("%i en el room 1\n",getpid());
		mutex_signal(semid,MUTEX);

		sem_wait(semid,T1,1);
			Rooms->room2 += 1;
			mutex_wait(semid,MUTEX);
			Rooms->room1 -= 1;
			if (Rooms->room1==0)
			{
				mutex_signal(semid,MUTEX);
				sem_signal(semid,T2,1);
				printf("%i dice: Vamonos al cuarto 2\n",getpid());
			} else {
				mutex_signal(semid,MUTEX);
				sem_signal(semid,T1,1);
				printf("%i dice: Me quedo en el cuarto 1\n",getpid());
			}

		sem_wait(semid,T2,1);
			Rooms->room2 -= 1;
			printf("%i dice: Estoy ejecutando mi seccion critica\n",getpid());
			sleep(pelo_crece);
			if (Rooms->room2 == 0)
			{
				sem_signal(semid,T1,1);
			} else {
				sem_signal(semid,T2,1);
			}
	}

	shmdt(Rooms);
}

int main(int argc, char *argv[])
{	
	int pid,i;
	if (argc != 2)
	{
		printf("forma de uso: %s [sleep time]\n",argv[0]);
	}

	for (i = 0; i < 5; ++i)
	{
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			return -1;
		} else if (pid == 0) {
			cliente(atoi(argv[1]));
		} else {
		}
	}
	return 0;
}