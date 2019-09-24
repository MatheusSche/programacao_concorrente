#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/*---------------------------------------------------------------------*/
double calcula_pi(double ini, double salto, double nr){
	double x, base, altura;
	double i, pi;

	base = 1.0 / nr; 
        pi = 0.0; 

       	for( i=ini ; i<=nr ;  i+=salto ) { 
                x = base * (( double)i - 0.5); 
                altura = 4.0 / (1.0 + x*x); 
		pi += base * altura;
	}

	return pi;


}
/*---------------------------------------------------------------------*/
int main(int argc, char **argv){

	double pi = 0, total;
	double nr;
	double i;

	int id;
	int p[2];

	if ( argc != 2 ){
		printf("%s <num_retangulos>\n", argv[0]);
		exit(0);
	}

	
	nr = atof(argv[1]);

	pipe(p);

	id = fork();

	if ( id > 0 ){
		close(p[1]);
		total = calcula_pi(1, 2, nr);
		read(p[0],&pi, sizeof(double));
		total += pi;
		printf("Pi = %.15f\n", total);	
	}
	else{
		close(p[0]);
		pi = calcula_pi(2, 2, nr);
		write(p[1],&pi, sizeof(double));
		close(p[1]); 
	}

}
/*---------------------------------------------------------------------*/

