#include "semaforos.h"

int main(int argc, char* argv[]) {
	int tamanio = 0, semid, i;
	unsigned short valores_iniciales[3] = {1, 1, 0};
	unsigned short valores_finales[4];
	key_t key;
	
	if (argc != 2) {
		printf("forma de uso: %s tamanio\n", argv[0]);
		return -1;
	}
	
	tamanio = atoi(argv[1]);
	if (tamanio <= 0) {
		printf("%s: El tamanio debe ser un entero positivo\n", argv[0]);
		return -1;
	}
	
	if ( (key = ftok("/dev/null", 65)) == (key_t) -1 ) {
		perror("ftok");
		return -1;
	}
	
	if ( (semid = semget(key, 4, 0666 | IPC_CREAT))  < 0 ) {
		perror("semget");
		return -1;
	}
	
	semctl(semid, PRODUCTOR, SETVAL, 1);
	semctl(semid, CONSUMIDOR, SETVAL, 1);
	semctl(semid, OCUPADO, SETVAL, 0);
	semctl(semid, LIBRE, SETVAL, tamanio);
	
	semctl(semid, 0, GETALL, valores_finales);
	printf("valores de los semaforos: ");
	for (i = 0; i < 4; i++) {
		printf("%3i", valores_finales[i]);
	}
	printf("\n");
	return 0;
}