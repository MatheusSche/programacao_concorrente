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
	int p1[2], p2[2], p3[3];

	if ( argc != 2 ){
		printf("%s <num_retangulos>\n", argv[0]);
		exit(0);
	}

	
	nr = atof(argv[1]);

	pipe(p1);
	pipe(p2);
	pipe(p3);


	id = fork();

	if ( id > 0 ){
		id = fork();
		if ( id > 0 ){
			id = fork();

			if ( id > 0 ){
				close(p1[1]);
				close(p2[1]);
				close(p3[1]);

				total = calcula_pi(1, 4, nr);
				read(p1[0],&pi, sizeof(double));
				total += pi;
				read(p2[0],&pi, sizeof(double));
				total += pi;
				read(p3[0],&pi, sizeof(double));
				total += pi;
				printf("Pi = %.15f\n", total);	

				close(p1[0]);
				close(p2[0]);
				close(p3[0]);


			}
			else{
				close(p2[0]);
				close(p2[1]);
				close(p3[0]);
				close(p3[1]);
				close(p1[0]);

				pi = calcula_pi(2, 4, nr);
				write(p1[1],&pi, sizeof(double));
				close(p1[1]); 
			}
		}
		else{
			close(p1[0]);
			close(p1[1]);
			close(p3[0]);
			close(p3[1]);
			close(p2[0]);

			pi = calcula_pi(3, 4, nr);
			write(p2[1],&pi, sizeof(double));

			close(p2[1]); 
		}
	}
	else{
		close(p1[0]);
		close(p1[1]);
		close(p2[0]);
		close(p2[1]);
		close(p3[0]);

		pi = calcula_pi(4, 4, nr);
		write(p3[1],&pi, sizeof(double));

		close(p3[1]); 
	}

}
/*---------------------------------------------------------------------*/

