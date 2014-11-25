#ifndef SEMAFOROS_H
#define SEMAFOROS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define	MUTEX 	0
#define T1 1 
#define T2 2

struct rooms{
	int room1;
	int room2;
};

void mutex_wait(int semid, int sem_num) {
	struct sembuf op;
	
	op.sem_num = sem_num;
	op.sem_op = -1;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

void mutex_signal(int semid, int sem_num) {
	struct sembuf op;
	
	op.sem_num = sem_num;
	op.sem_op = 1;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

void sem_wait(int semid, int sem_num, int val) {
	struct sembuf op;
	
	op.sem_num = sem_num;
	op.sem_op = -val;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

void sem_signal(int semid, int sem_num, int val) {
	struct sembuf op;
	
	op.sem_num = sem_num;
	op.sem_op = val;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

#endif