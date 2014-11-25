#include "semaforos.h"
#include <time.h>

void un_consumidor() {
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
		printf("consumidor %i intentando entrar a almacen\n", getpid());
		mutex_wait(semid, CONSUMIDOR);
		printf("consumidor %i entro a almacen\n", getpid());
		
		i = (rand() % 5) + 1;
		printf("consumidor %i intentando tomar %i producto(s) - espacio ocupado = %i\n", getpid(), i, semctl(semid, OCUPADO, GETVAL, 0));
		sem_wait(semid, OCUPADO, i);
		printf("consumidor %i tomo sus productos\n", getpid(), i);
		sem_signal(semid, LIBRE, i);
		
		mutex_signal(semid, CONSUMIDOR);
		printf("consumidor %i se ira a dormir\n", getpid());
		
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
			un_consumidor();
		} else {
		}
	}
	return 0;
}