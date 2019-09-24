#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/msg.h>

/*----------------------------------------------------------*/
struct msg{
	long tipo;
	double cont;
};
typedef struct msg MSG;

/*----------------------------------------------------------*/
double calculo_pi(double n){
	double i, cont = 0;
	double x, y, distancia;

	srand(time(NULL));

	for ( i=0; i<n; i++){
		x = rand()/(double)RAND_MAX;
		y = rand()/(double)RAND_MAX;
		distancia = sqrt(x*x + y*y);
		if ( distancia <= 1 ){
			cont++;
		}
	}
	return cont;

}

/*----------------------------------------------------------*/
int main(int argc, char **argv ){
	
	double n, pi, cont;
	MSG mensagem;
	int fila, chave = 4;
	int pid;
	
	if ( argc != 2 ){
		printf("%s < num_pontos >\n", argv[0]);
		exit(0);
	}

	n = atof(argv[1]);

	fila = msgget(chave, 0600 | IPC_CREAT);

	pid = fork();

	if ( pid > 0 ){
		pid = fork();
		if ( pid > 0 ){
			pid = fork();

			if ( pid > 0 ) {
				
				cont = calculo_pi(n/4);

				msgrcv(fila, &mensagem, sizeof(mensagem.cont), 0, 0);
				cont += mensagem.cont;
				msgrcv(fila, &mensagem, sizeof(mensagem.cont), 0, 0);
				cont += mensagem.cont;
				msgrcv(fila, &mensagem, sizeof(mensagem.cont), 0, 0);
				cont += mensagem.cont;
		
				pi = 4 * cont/n;

				printf("PI: %.15f\n", pi);

				msgctl(fila, IPC_RMID, 0);
			}
			else{
				mensagem.tipo = 3;
				mensagem.cont = calculo_pi(n/4);
				msgsnd(fila, &mensagem, sizeof(mensagem.cont), 0);
			}
		}
		else{
			mensagem.tipo = 2;
			mensagem.cont = calculo_pi(n/4);
			msgsnd(fila, &mensagem, sizeof(mensagem.cont), 0);
		}		
	}
	else{
		mensagem.tipo = 1;
		mensagem.cont = calculo_pi(n/4);
		msgsnd(fila, &mensagem, sizeof(mensagem.cont), 0);
	}	
	
}
/*----------------------------------------------------------*/

