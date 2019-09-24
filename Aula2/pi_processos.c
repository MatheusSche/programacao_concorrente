#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
/*---------------------------------------------------------------------*/
double calcula_pi(int ini, int nproc,  double nr){
	double x, base, altura;
	double i, pi;

	base = 1.0 / nr; 
        pi = 0.0; 

       	for( i=ini ; i<=nr ;  i+=nproc ) { 
                x = base * (( double)i - 0.5); 
                altura = 4.0 / (1.0 + x*x); 
		pi += base * altura;
	}

	return pi;


}
/*---------------------------------------------------------------------*/
int main(int argc, char **argv){

	int p1[2], p2[2], p3[2]; //PIPES
	double pi = 0;
	double nr, aux;
	double i;
    int pid;


	if ( argc != 2 ){
		printf("%s <num_retangulos>\n", argv[0]);
		exit(0);
	}

	
	nr = atof(argv[1]);

	pipe(p1);
	pipe(p2);
	pipe(p3);
	
	
	pid = fork();

	if( pid > 0 ){
		
		pid = fork();

		if(pid > 0 ){
			
			pid = fork();

			if(pid > 0 ){
				close(p1[1]);
				close(p2[1]);
				close(p3[1]);
				
				pi = calcula_pi(4,4,nr);
				printf("Pai encontrou Pi = %.15f\n",pi);

				read(p1[0], &aux, sizeof(double));
			
				pi+=aux;

				read(p2[0], &aux, sizeof(double));

				pi+=aux;

				read(p3[0], &aux, sizeof(double));

				pi+=aux;
				
				printf("Pi final == %.15f\n", pi);
				
				close(p1[0]);
				close(p2[0]);
				close(p3[0]);
			} else {
				//TERCEIRO FILHO

				//FECHA O PIPE 1
				close(p1[0]);
				close(p1[1]);

				//FECHA O PIPE2
				close(p2[0]);
				close(p2[1]);
				
				close(p3[0]);

				pi = calcula_pi(3,4,nr);
				
				printf("Filho 3 encontrou Pi = %.15f\n",pi);
			
				write(p3[1], &pi, sizeof(double));
			
				close(p3[1]);
			}

		} else {
			//SEGUNDO FILHO
			
			//FECHA O PIPE 1
			close(p1[0]);
			close(p1[1]);

			//FECHA O PIPE3
			close(p3[0]);
			close(p3[1]);
			
			close(p2[0]);

			pi = calcula_pi(2,4,nr);
			
			printf("Filho 2 encontrou Pi = %.15f\n",pi);
		
			write(p2[1], &pi, sizeof(double));
		
			close(p2[1]);

		}
		
		
		

	} else {
		//PRIMEIRO FILHO
		
		//FECHA O PIPE 2
		close(p2[0]);
		close(p2[1]);
		
		//FECHA O PIPE3
		close(p3[0]);
		close(p3[1]);
		
		close(p1[0]);
		
		pi = calcula_pi(1,4,nr);
		printf("Filho 1 encontrou Pi = %.15f\n",pi);
		
		write(p1[1], &pi, sizeof(double));
		
		close(p1[1]);
	}
	
		
}
/*---------------------------------------------------------------------*/

