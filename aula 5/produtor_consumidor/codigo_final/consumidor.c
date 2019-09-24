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
	
	FILE *out;
	char *buffer;

	int shm1, shm2;
	int chave_shm1 = 3, chave_shm2 = 5;
	int chave_sem = 7;
	int *bytesLidos;
	int sem;

	struct sembuf semaforo;
		
	if ( argc != 2 ){
		printf("%s <arquivo_copia>\n", argv[0]);
		exit(0);
	}

	out = fopen(argv[1], "wb");

	shm1 = shmget(chave_shm1, TAMMAX, 0);
	buffer = shmat(shm1, 0, 0);

	shm2 = shmget(chave_shm2, sizeof(int), 0);
	bytesLidos = shmat(shm2, 0, 0);

	sem = semget(chave_sem, 0, 0);
	printf("SEM0: %d\n", semctl(sem, 0, GETVAL));
	printf("SEM1: %d\n", semctl(sem, 1, GETVAL));

	do{
		
		P(sem, semaforo, 1);
		
		fwrite(buffer, sizeof(char), *bytesLidos, out);

		V(sem, semaforo, 0);
		
	}
	while(*bytesLidos > 0);

	shmdt(buffer);
	shmdt(bytesLidos);

	shmctl(shm1, IPC_RMID, 0);
	shmctl(shm2, IPC_RMID, 0);
	semctl(sem, IPC_RMID, 0);

	fclose(out);
 	
}	
/*-------------------------------------------------------------------------*/
