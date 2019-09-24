#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>

/*-------------------------------------------------------------*/
#define N 5

/*-------------------------------------------------------------*/
void inicializa_matriz(float mat[N][N]){
	int i,j;

	//srand(time(NULL));
	for ( i=0; i<N; i++ ){
		for( j=0; j<N; j++ ){
			mat[i][j] = rand()%10;
		}
	}
}
/*-------------------------------------------------------------*/
void escreve_matriz(float mat[N][N]){
	int i,j;

	for ( i=0; i<N; i++ ){
		for( j=0; j<N; j++ ){
			printf("% 5.2f", mat[i][j]);			
		}
		printf("\n");
	}
	printf("\n\n");
}
/*-------------------------------------------------------------*/
void escreve_matrizC(float *mat){
	int i,j;

	for ( i=0; i<N; i++ ){
		for( j=0; j<N; j++ ){
			printf("% 5.2f", mat[i*N+j]);			
		}
		printf("\n");
	}
	printf("\n\n");
}
/*-------------------------------------------------------------*/
void multiplica_matriz(int ini, int np, float matA[N][N], float matB[N][N], float *matC){
	int i,j,k;
	for ( i=ini; i<N; i+=np ){
		for( j=0; j<N; j++ ){
			matC[i*N+j] = 0;
			for( k=0; k<N; k++ ){
				matC[i*N+j] += matA[i][k] * matB[k][j];
			}
		}
	}
}
/*-------------------------------------------------------------*/

int main(int argc, char **argv){
	
	float matA[N][N], matB[N][N];
	int pid;
	int shmid, chave = 5;
	float *matC = NULL;

	shmid = shmget(chave, N*N*sizeof(float), 0600 | IPC_CREAT);

	matC = shmat(shmid, 0, 0);
	
	inicializa_matriz(matA);
	inicializa_matriz(matB);

	escreve_matriz(matA);
	escreve_matriz(matB);
	

	pid = fork();

	if ( pid > 0 ){
		pid = fork();
		
		if ( pid > 0 ){
			multiplica_matriz(2, 3, matA, matB, matC);
			wait(NULL);
			wait(NULL);
			escreve_matrizC(matC);
			shmdt(matC);
			shmctl(shmid, IPC_RMID, 0);
		}
		else{
			multiplica_matriz(1, 3, matA, matB, matC);
			shmdt(matC);
		}
	}
	else{
		
		multiplica_matriz(0, 3, matA, matB, matC);
		shmdt(matC);
	}
	
}
