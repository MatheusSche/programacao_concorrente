#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/msg.h>


/*-------------------------------------------------------------------------*/
#define N 10
/*-------------------------------------------------------------------------*/
struct msg{
	long tipo;
	float soma;
	float soma2;
};
typedef struct msg MSG;

/*-------------------------------------------------------------------------*/
void gera_vetor(float vet[N]){
	int i;
	for (i=0; i<N; i++){
		vet[i] = rand()%10;
	}
}

/*-------------------------------------------------------------------------*/
void escreve_vetor(float vet[N]){
	int i;
	for (i=0; i<N; i++){
		printf("%f\n", vet[i]);
	}
}

/*-------------------------------------------------------------------------*/
float executa_somas(int ini, int np, float vet[N], float *soma, float *soma2){
	
	*soma = 0;
	*soma2 = 0;
	int i;

	for(i=ini; i<N; i+=np){
		*soma = *soma + vet[i];
		*soma2 = *soma2 + vet[i]*vet[i];
	}
}

/*-------------------------------------------------------------------------*/
void calcula_media_desvio(float soma, float soma2, float *media, float *dsv){

	*media = soma / N;
	
	*dsv = sqrt( soma2/N - (*media * *media) );
}

/*-------------------------------------------------------------------------*/
int main(int argc, char **argv){
	MSG m;
	int pid, msgid;
	int chave = 7;
	float soma, soma2;
	float media, dsv;
	float vet[N];

	gera_vetor(vet);
	escreve_vetor(vet);

	msgid = msgget(chave, 0600 | IPC_CREAT);

	pid = fork();

	if ( pid > 0 ){
		executa_somas(1, 2, vet,  &soma, &soma2);
		msgrcv(msgid, &m, sizeof(m.soma) + sizeof(m.soma2),0,0);
		soma += m.soma;
		soma2 += m.soma2;
		calcula_media_desvio(soma, soma2, &media, &dsv);
		printf("Media= %f\nDesvio Padrao: %f\n", media, dsv);
		msgctl(msgid, IPC_RMID, 0);
	}
	else{
		m.tipo = 1;
		executa_somas(0, 2, vet, &soma, &soma2);
		m.soma = soma;
		m.soma2 = soma2;
		msgsnd(msgid, &m, sizeof(m.soma) + sizeof(m.soma2), 0);
	}
	
}
/*-----------------------------------------------------*/

