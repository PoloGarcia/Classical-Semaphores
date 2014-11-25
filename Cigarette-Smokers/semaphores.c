#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define	TABLE		0
#define BARRETTE	1
#define	RHINOSTONE	2
#define FLOWER		3

int mutex_wait(int semid, int sem_num) {
	struct sembuf op;
	
	op.sem_num = sem_num;
	op.sem_op = -1;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

int mutex_signal(int semid, int sem_num) {
	struct sembuf op;
	
	op.sem_num = sem_num;
	op.sem_op = 1;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

#endif