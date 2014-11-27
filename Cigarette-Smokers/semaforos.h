#ifndef SEMAFOROS_H
#define SEMAFOROS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define	MUTEX 		0
#define TOBACCO 	1 
#define TOBACCO_SEM	2
#define PAPER 		3
#define PAPER_SEM	4
#define MATCHES		5
#define MATCHES_SEM	6
#define AGENT		7

struct table{
	int isTobacco;
	int isPaper;
	int isMatches; 
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