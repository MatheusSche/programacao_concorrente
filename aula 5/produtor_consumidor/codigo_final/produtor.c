#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/sem.h>
	
/*-------------------------------------------------------------------------*/
#define TAMMAX 200

/*------------------------------------------------------------------------*/
void P(int semid, struct sembuf semaforo, int num){
	semaforo.sem_num = num;
	semaforo.sem_op = -1;
	semaforo.sem_flg = SEM_UNDO;
	semop(semid, &semaforo, 1); 
}

/*------------------------------------------------------------------------*/
void V(int semid, struct sembuf semaforo, int num){
	semaforo.sem_num = num;
	semaforo.sem_op = 1;
	semaforo.sem_flg = SEM_UNDO;
	semop(semid, &semaforo, 1); 
}
	
/*-------------------------------------------------------------------------*/
void main(int argc, char **argv){
	
	FILE *in;
	char *buffer;

	int shm1, shm2;
	int chave_shm1 = 3, chave_shm2 = 5;
	int chave_sem = 7;
	int sem;

	int *bytesLidos;

	struct sembuf semaforo;

	if ( argc != 2 ){
		printf("%s <arquivo_original>\n", argv[0]);
		exit(0);
	}

	in  = fopen(argv[1], "rb");

	shm1 = shmget(chave_shm1, TAMMAX, 0600 | IPC_CREAT);
	buffer = shmat(shm1, 0, 0);

	shm2 = shmget(chave_shm2, sizeof(int), 0600 | IPC_CREAT);
	bytesLidos = shmat(shm2, 0, 0);

	sem = semget(chave_sem, 2, 0600 | IPC_CREAT);
	semctl(sem, 0, SETVAL, 1);
	semctl(sem, 1, SETVAL, 0);
 
	do {

		P(sem, semaforo, 0);

		*bytesLidos = fread(buffer, sizeof(char), TAMMAX, in);		

		V(sem, semaforo, 1);

	}
	while( *bytesLidos > 0);
			
	shmdt(buffer);
	shmdt(bytesLidos);

	fclose(in);

}	
/*-------------------------------------------------------------------------*/
