#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
/*-----------------------------------------------------*/
double  pi(int ini, int nproc, double n){
	double termo, p = 0;
	double i;
	for( i=ini; i<=n; i+=nproc ){
		termo = 1.0 / (2*i-1);
		if ( fmod(i,2) != 0 ){ 		
			p += termo;
		}
		else{
			p -= termo;
		} 
	}

	p *= 4;
	return(p);
}

/*-----------------------------------------------------*/
int main(int argc, char **argv){
	double ntermos;
	int p1[2], p2[2];
	int pid;
	double calc_pi = 0, aux=0;
	
	
	if ( argc != 2 ){
		printf("%s <num_termos>\n", argv[0]);
		exit(0); 
	}
	
	ntermos = atof(argv[1]);

	pipe(p1);
	pipe(p2);
	
	pid = fork();

	if(pid > 0){

		pid = fork();
		if(pid > 0){
			close(p1[1]);
			close(p2[1]);

			calc_pi = pi(1,3,ntermos);
			//pegou os resultados do filho 1
			read(p1[0], &aux, sizeof(double));
			calc_pi+=aux;
			//pegou resultados com o filho 2
			read(p2[0], &aux, sizeof(double));
			calc_pi+=aux;
			
			printf("Pi final = %.15f\n", calc_pi);

			close(p1[0]);
			close(p2[0]);

		} else {
			//filho 2
			//fecha a conexão de leitura com o pipe do pai
			close(p2[0]);


			//fecha conexão total com o irmão 1
			close(p1[0]);
			close(p1[1]);

			calc_pi = pi(3,3,ntermos);
			write(p2[1], &calc_pi, sizeof(double));

			close(p2[1]);
		}	

	} else {
		//filho 1
		//fecha conexão de leitura com o pipe do pai
		close(p1[0]); 

		//fecha a conexão total com o irmão 2
		close(p2[0]);
		close(p2[1]);

		calc_pi = pi(2,3,ntermos);
		write(p1[1], &calc_pi, sizeof(double));
		
		close(p1[1]);
	}
}
/*-----------------------------------------------------*/

