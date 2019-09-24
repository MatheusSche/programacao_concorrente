#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>

/*-----------------------------------------------*/
#define N 100000

/*-----------------------------------------------*/
void gera_vetor(int vet[N]){
	int i;
	srand(time(NULL));		
	for (i=0;i<N;i++){
		vet[i] = rand()%10;
	}
}
/*-----------------------------------------------*/
void ordena_vetor(int ini, int salto, int vet[N], int ordenado[N]){
	int i, j, cont;

	for ( i=ini; i<N; i+=salto ){
		cont = 0;
		for ( j=0; j<N; j++ ){
			if (  (vet[j] < vet[i]) ||  (vet[j] == vet[i] && i < j ) ){
				cont++;			
			}		
		}
		ordenado[cont] = vet[i];
	}

}

/*-----------------------------------------------*/
void escreve_vetor(int vet[N]){
	int i;
	for (i=0;i<N;i++){
		printf("%d ", vet[i]); 	
	}
	printf("\n");
}

/*-----------------------------------------------*/
int main(){
	int vet[N];
	int *ordenado = NULL;
	int shm;
	int chave = 7;
	int pid;
	
	//escreve o vetor
	gera_vetor(vet);

	shm = shmget(chave, N*sizeof(int), 0600 | IPC_CREAT);

	ordenado = shmat(shm, 0, 0);

	pid = fork();

	if(pid > 0){
		pid = fork();
		if(pid > 0){
			ordena_vetor(0,3,vet,ordenado);
			wait(NULL);
			wait(NULL);
			
			escreve_vetor(ordenado);
			shmdt(ordenado);
			shmctl(shm, IPC_RMID,0);

		} else {
			ordena_vetor(2,3,vet,ordenado);
			shmdt(ordenado);
		}
		
	} else {
		ordena_vetor(1,3,vet,ordenado);
		shmdt(ordenado);
	}
	
	
//	escreve_vetor(ordenado);
}
/*-----------------------------------------------*/
