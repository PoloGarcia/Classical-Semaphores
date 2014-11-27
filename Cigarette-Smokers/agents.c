#include "semaforos.h"
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>
/*

*/

void agent_a(){
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
		sem_wait(semid,AGENT,1);
		sem_signal(semid,TOBACCO,1);
		sem_signal(semid,PAPER,1);
	}

	shmdt(Table);
	exit(0);
}

void agent_b(){
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
		sem_wait(semid,AGENT,1);
		sem_signal(semid,PAPER,1);
		sem_signal(semid,MATCHES,1);
	}

	shmdt(Table);
	exit(0);
}

void agent_c(){
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
		sem_wait(semid,AGENT,1);
		sem_signal(semid,TOBACCO,1);
		sem_signal(semid,MATCHES,1);
	}

	shmdt(Table);
	exit(0);
}

void pusher_a(){
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

	/*
	tobacco.wait()
	mutex.wait()
	     if isPaper:
	        isPaper = False
	        matchSem.signal()
	     elif isMatch:
	        isMatch = False
	        paperSem.signal()
	     else:
	        isTobacco = True
	mutex.signal()
	*/
	while(1) {
		printf("TOBACCO on the table cheking for PAPER and MATCHES\n");
		sem_wait(semid,TOBACCO,1);
		mutex_wait(semid,MUTEX);
			if (Table->isPaper)
			{
				Table->isPaper--;
				sem_signal(semid,MATCHES_SEM,1);
			}else if (Table->isMatches){
				Table->isMatches--;
				sem_signal(semid,PAPER_SEM,1);
			} else {
				Table->isTobacco++;
			}
		mutex_signal(semid,MUTEX);
		sleep(3);
	}

	shmdt(Table);
	exit(0);
}

void pusher_b(){
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
		printf("PAPER on the table cheking for MATCHES and TOBACCO\n");
		sem_wait(semid,PAPER,1);
		mutex_wait(semid,MUTEX);
			if (Table->isTobacco)
			{
				Table->isTobacco--;
				sem_signal(semid,MATCHES_SEM,1);
			}else if (Table->isMatches){
				Table->isMatches--;
				sem_signal(semid,TOBACCO_SEM,1);
			} else {
				Table->isPaper++;
			}
		mutex_signal(semid,MUTEX);
		sleep(3);
	}
	
	shmdt(Table);
	exit(0);
}

void pusher_c(){
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
		printf("MATCHES on the table cheking for PAPER and TOBACCO\n");
		sem_wait(semid,MATCHES,1);
		mutex_wait(semid,MUTEX);
			if (Table->isTobacco)
			{
				Table->isTobacco--;
				sem_signal(semid,PAPER_SEM,1);
			}else if (Table->isPaper){
				Table->isPaper--;
				sem_signal(semid,TOBACCO_SEM,1);
			} else {
				Table->isMatches++;
			}
		mutex_signal(semid,MUTEX);
		sleep(3);
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
		pusher_a();
	}

	if ( (pid = fork()) < 0 ) {
		perror("fork2");
		return -1;
	} else if (pid == 0) {
		pusher_b();
	}

	if ( (pid = fork()) < 0 ) {
		perror("fork3");
		return -1;
	} else if (pid == 0) {
		pusher_c();
	}

	if ( (pid = fork()) < 0 ) {
		perror("fork4");
		return -1;
	} else if (pid == 0) {
		agent_a();
	}

	if ( (pid = fork()) < 0 ) {
		perror("fork5");
		return -1;
	} else if (pid == 0) {
		agent_b();
	}

	if ( (pid = fork()) < 0 ) {
		perror("fork6");
		return -1;
	} else if (pid == 0) {
		agent_c();
	}

	return 0;
}