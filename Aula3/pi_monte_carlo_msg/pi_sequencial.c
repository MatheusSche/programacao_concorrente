#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

/*----------------------------------------------------------*/
double calculo_pi(double n){
	double i, cont = 0;
	double x, y, distancia;
	double pi;

	srand(time(NULL));

	for ( i=0; i<n; i++){
		x = rand()/(double)RAND_MAX;
		y = rand()/(double)RAND_MAX;
		distancia = sqrt(x*x + y*y);
		if ( distancia <= 1 ){
			cont++;
		}
	}
	pi = 4 * cont/n;

	return pi;

}

/*----------------------------------------------------------*/
int main(int argc, char **argv ){
	double n, pi;
	
	if ( argc != 2 ){
		printf("%s < num_pontos >\n", argv[0]);
		exit(0);
	}

	n = atof(argv[1]);

	pi = calculo_pi(n);

	printf("PI: %.15f\n", pi);

}
/*----------------------------------------------------------*/

