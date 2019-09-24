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
	
	FILE *in;
	char *buffer;

	int shm1, shm2;
	int chave_shm1 = 3, chave_shm2 = 5;

	int chave_sem = 7;
	int S0 = 0, S1 = 1;
	int semid;
	
	int *bytesLidos;

	if ( argc != 2 ){
		printf("%s <arquivo_original>\n", argv[0]);
		exit(0);
	}

	in  = fopen(argv[1], "rb");

	shm1 = shmget(chave_shm1, TAMMAX, 0600 | IPC_CREAT);
	buffer = shmat(shm1, 0, 0);

	shm2 = shmget(chave_shm2, sizeof(int), 0600 | IPC_CREAT);
	bytesLidos = shmat(shm2, 0, 0);


	semid = semget(chave_sem, 2, 0600 | IPC_CREAT);//cria um array de semáforos
	semctl(semid, 0, SETVAL, 0);
	semctl(semid, 1, SETVAL, 1);

	printf("SO: %d\n", semctl(semid,0,GETVAL));
	printf("S1: %d\n", semctl(semid,1,GETVAL));
	
	do {

		P(semid, 1);
		//id array, posição do array

		*bytesLidos = fread(buffer, sizeof(char), TAMMAX, in);		
			
		V(semid, 0);
		//id array, posição do array
	}
	while( *bytesLidos > 0);

	shmdt(buffer);
	shmdt(bytesLidos);

	fclose(in);

}	
/*-------------------------------------------------------------------------*/
