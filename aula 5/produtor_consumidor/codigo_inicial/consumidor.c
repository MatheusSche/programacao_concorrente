#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
	
/*-------------------------------------------------------------------------*/
#define TAMMAX 200

/*--------------------------------------------------------------------------*/
void P(int semid, int num_sem){

	struct  sembuf op;

	op.sem_num = num_sem;
	op.sem_op = -1; // ato de decrementar o semaforo
	op.sem_flg = SEM_UNDO; //quando programa crash, libera o semaforo, volta ao estado anterior
	semop(semid, &op, 1);	
		
}
/*-------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
void V(int semid, int num_sem){

	struct sembuf op;

	op.sem_num = num_sem;
	op.sem_op = 1; // ato de incrementar o semaforo
	op.sem_flg = SEM_UNDO; //quando programa crash, libera o semaforo, volta ao estado anterior
	semop(semid, &op, 1); //executar a minha operação

}
/*-------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------*/
void main(int argc, char **argv){
	
	FILE *out;
	char *buffer;

	int shm1, shm2;
	int chave_shm1 = 3, chave_shm2 = 5;
	int *bytesLidos;
	int chave_sem = 7;
	int semid;

		
	if ( argc != 2 ){
		printf("%s <arquivo_copia>\n", argv[0]);
		exit(0);
	}

	out = fopen(argv[1], "wb");

	shm1 = shmget(chave_shm1, TAMMAX, 0); // 0 = vai se conectar a uma que já existe
	buffer = shmat(shm1, 0, 0);

	shm2 = shmget(chave_shm2, sizeof(int), 0);
	bytesLidos = shmat(shm2, 0, 0);

	semid = semget(chave_sem,2,0);

	printf("SO: %d\n", semctl(semid,0,GETVAL));
	printf("S1: %d\n", semctl(semid,1,GETVAL));


	do{

		P(semid, 0);

		fwrite(buffer, sizeof(char), *bytesLidos, out);

		V(semid, 1);

	}
	while(*bytesLidos > 0);

	shmdt(buffer);
	shmdt(bytesLidos);

	shmctl(shm1, IPC_RMID, 0);
	shmctl(shm2, IPC_RMID, 0);

	fclose(out);
 	
}	
/*-------------------------------------------------------------------------*/
