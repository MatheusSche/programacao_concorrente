#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>

/*-----------------------------------------*/
#define N 10

struct msg{
	long tipo;
	float soma1;
	float soma2;
};
typedef struct msg MSG;

/*-----------------------------------------*/
void gera_vetor(float vet[N]){
	int i;
	for (i=0; i<N; i++){
		vet[i] = rand()%10;
	}
}

/*-----------------------------------------*/
void escreve_vetor(float vet[N]){
	int i;
	for (i=0; i<N; i++){
		printf("%f\n", vet[i]);
	}
}
/*-----------------------------------------*/
float media_desvio_padrao(float vet[N], float *soma1, float *soma2, int ini, int salto){
	int i;
	*soma1 = 0;
	*soma2 = 0;

	for(i=ini; i<N; i+=salto){
		*soma1 = *soma1 + vet[i];
		*soma2 = *soma2 + vet[i]*vet[i];
	}

}
/*-----------------------------------------*/
int main(){
	float vet[N];
	float media=0, desvio=0;
	MSG mensagem;
	int fila, chave = 4;
	int pid;
	float soma1=0, soma2=0;
	int nprocessos;

	gera_vetor(vet);
	escreve_vetor(vet);
	//fila criada
	fila = msgget(chave, 0600 | IPC_CREAT);

	pid = fork();

	if(pid > 0) {
		
		pid = fork();
		
		if(pid>0){
			media_desvio_padrao(vet, &soma1, &soma2, 0,3);
	
			msgrcv(fila, &mensagem, (sizeof(float)+sizeof(float)), 0, 0);
			
			soma1+=mensagem.soma1;
			soma2+=mensagem.soma2;

			msgrcv(fila, &mensagem, (sizeof(float)+sizeof(float)), 0, 0);
			
			soma1+=mensagem.soma1;
			soma2+=mensagem.soma2;
			
			//o pai fará isso aqui
			media = soma1 / N;
			desvio = sqrt(soma2/N - media * media); 

			printf("Media: %f\n", media);
			printf("Desvio: %f\n", desvio);

			msgctl(fila, IPC_RMID, 0);
		} else {
			mensagem.tipo = 2;
			media_desvio_padrao(vet,&soma1, &soma2, 2,3);
			
			mensagem.soma1 = soma1;
			mensagem.soma2 = soma2;

			msgsnd(fila, &mensagem, (sizeof(float)+sizeof(float)), 0);

		}
		
	
	} else {
		mensagem.tipo = 1;
		media_desvio_padrao(vet,&soma1, &soma2, 1,3);
		
		mensagem.soma1 = soma1;
		mensagem.soma2 = soma2;

		msgsnd(fila, &mensagem, (sizeof(float)+sizeof(float)), 0);

	}
	
	
	
}
