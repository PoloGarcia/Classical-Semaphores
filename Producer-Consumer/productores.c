#include "semaforos.h"
#include <time.h>

void un_productor() {
	int semid, i, k;
	key_t key;
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("ftok");
		exit(-1);
	}
	
	if ( (semid = semget(key, 4, 0666))  < 0 ) {
		perror("semget");
		exit(-1);
	}
	
	srand( getpid() );
	k = 10;
	while (k-- > 0) {
		printf("productor %i intentando entrar a almacen\n", getpid());
		mutex_wait(semid, PRODUCTOR);
		printf("productor %i entro a almacen\n", getpid());
		
		i = (rand() % 5) + 1;
		printf("productor %i intentando colocar %i producto(s) - espacio libre = %i\n", getpid(), i, semctl(semid, LIBRE, GETVAL, 0));
		sem_wait(semid, LIBRE, i);
		printf("productor %i coloco sus productos\n", getpid(), i);
		sem_signal(semid, OCUPADO, i);
		
		mutex_signal(semid, PRODUCTOR);
		printf("productor %i se ira a dormir\n", getpid());
		
		sleep((rand() % 10) + 1);
	}
	exit(0);
}

int main(int argc, char* argv[]) {
	int cantidad = 0, semid, i, pid;
	key_t key;
	
	if (argc != 2) {
		printf("forma de uso: %s cantidad\n", argv[0]);
		return -1;
	}
	
	cantidad = atoi(argv[1]);
	if (cantidad <= 0) {
		printf("%s: La cantidad debe ser un entero positivo\n", argv[0]);
		return -1;
	}
	
	for (i = 0; i < cantidad; i++) {
		if ( (pid = fork()) < 0 ) {
			perror("fork");
			return -1;
		} else if (pid == 0) {
			un_productor();
		} else {
		}
	}
	return 0;
}