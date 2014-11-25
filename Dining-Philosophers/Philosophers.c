#include "semaforos.h"
#include <time.h>

int forks_array[FOOTMAN] ={FORK_01,FORK_02,FORK_03,FORK_04,FORK_05};

int left(int i){
	return i;
}

int right(int i){
	return (i+1) % 5;
}

void get_forks(int semid,int i){
	sem_wait(semid, FOOTMAN, 1);
	sem_wait(semid,forks_array[right(i)],1);
	sem_wait(semid,forks_array[left(i)],1);
}

void put_forks(int semid, int i){
	sem_signal(semid,forks_array[right(i)],1);
	sem_signal(semid,forks_array[left(i)],1);
	sem_signal(semid, FOOTMAN, 1);
}

void philosopher(int id) {
	int semid, k;
	key_t key;
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("ftok");
		exit(-1);
	}
	
	if ( (semid = semget(key, 6, 0666))  < 0 ) {
		perror("semget");
		exit(-1);
	}
	
	srand( getpid() );

	while(1){
		k = rand()%5;
		printf("(╯°□°）╯ %d - ‎I'm thinking, I'll try to eat in %i seconds\n", id, k);
		sleep(k);
		get_forks(semid,id);
		int l,r;
		l = left(id);
		r = right(id);
		printf("(╯°□°）╯ %d - ‎I'm GOT the forks, left: %i right: %i \n", id, l,r);
		k = rand()%5;
		printf("(╯°□°）╯ %d - ‎I'm going to eat: for %i seconds\n", id,k);
		sleep(k);
		put_forks(semid,id);
		printf("(╯°□°）╯ %d - ‎I'm PUT the forks, left: %i right: %i \n", id, l,r);
	}
	exit(0);
}

int main(int argc, char* argv[]) {
	int cantidad = 0, semid, i, pid;
	key_t key;
	
	if (argc != 1) {
		printf("forma de uso: %s", argv[0]);
		return -1;
	}
	

	for (i = 0; i < 5; i++) {
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			return -1;
		} else if (pid == 0) {
			philosopher(i);
		} else {
		}
	}
	return 0;
}