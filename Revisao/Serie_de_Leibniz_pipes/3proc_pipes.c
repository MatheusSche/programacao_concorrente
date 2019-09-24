#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

/*-----------------------------------------------------*/
double  pi(double ini, double nproc, double n){
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
	double ntermos, aux, p;
	int p1[2], p2[2];
	int pid;


	if ( argc != 2 ){
		printf("%s <num_termos>\n", argv[0]);
		exit(0); 
	}

	ntermos = atof(argv[1]);

	pipe(p1);
	pipe(p2);

	pid = fork();

	if ( pid > 0 ){

		pid = fork();

		if ( pid > 0 ) {
			close(p1[1]);
			close(p2[1]);
			p = pi(3, 3, ntermos);
			read(p1[0], &aux, sizeof(double));
			p+= aux;
			read(p2[0], &aux, sizeof(double));
			p+= aux;

			close(p1[0]);
			close(p2[0]);

			printf("PI= %.15f\n", p);
		}
		else{
			close(p1[0]);
			close(p1[1]);
			close(p2[0]);
			p = pi(2, 3, ntermos);
			write(p2[1], &p, sizeof(double));
			close(p2[1]);
		}
	}
	else{
		close(p1[0]);
		close(p2[0]);
		close(p2[1]);
		p = pi(1, 3, ntermos);
		write(p1[1], &p, sizeof(double));
		close(p1[1]);
	}

	
}
/*-----------------------------------------------------*/

