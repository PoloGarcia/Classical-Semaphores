#ifndef SEMAFOROS_H
#define SEMAFOROS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

/*Valores de semaforos consistentes con numero 
de localidad del arreglo de semaforos*/
#define	FORK_01 	0
#define FORK_02		1
#define	FORK_03		2
#define FORK_04		3
#define FORK_05		4

/*Cadenero :P*/
#define FOOTMAN		5

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

int sem_wait(int semid, int sem_num, int val) {
	struct sembuf op;
	
	op.sem_num = sem_num;
	op.sem_op = -val;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

int sem_signal(int semid, int sem_num, int val) {
	struct sembuf op;
	
	op.sem_num = sem_num;
	op.sem_op = val;
	op.sem_flg = 0;
	semop(semid, &op, 1);
}

#endif