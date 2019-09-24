#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/shm.h>
#include <unistd.h>

/*-----------------------------------------------*/
#define N 10000

/*-----------------------------------------------*/
void gera_vetor(int vet[N]){
	int i;
	srand(time(NULL));		
	for (i=0;i<N;i++){
		vet[i] = rand()%10;
	}
}
/*-----------------------------------------------*/
void ordena_vetor(int ini, int salto, int vet[N], int *ordenado){
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
	int shmid;
	int chave = 3;
	int id;

	gera_vetor(vet);
	
	shmid = shmget(chave, N*sizeof(int), 0600 | IPC_CREAT);
	//777 = leitura e escrita para todos os processo

	ordenado = shmat(shmid,0,0);

	id = fork();

	if(id > 0){
		ordena_vetor(1, 2, vet, ordenado);
		wait(NULL);
		escreve_vetor(ordenado);
		shmdt(ordenado);
		shmctl(shmid, IPC_RMID, 0);
	} else {
		
		ordena_vetor(0, 2, vet, ordenado);
		shmdt(ordenado);
	}
//	printf("%d\n", shmid);
//	escreve_vetor(vet);
//  ordena_vetor(vet,ordenado);
//	escreve_vetor(ordenado);
}
/*-----------------------------------------------*/
