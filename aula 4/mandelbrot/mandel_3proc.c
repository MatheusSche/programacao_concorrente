#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define buffer(y,x) buffer[y*nx + x]

int max_iterations = 255;

//*******************************************************************************//

int calcula_ponto(double ci, double cr) {
  int iteracoes = 0;
  double zi = 0;
  double zr = 0;
  
  while ((zr*zr + zi*zi < 4) && (iteracoes < max_iterations)) {
    double nr, ni; 

    /* Z <-- Z^2 + C */
   
    nr = zr*zr - zi*zi + cr;
    ni = 2*zr*zi + ci;
    
    zi = ni;
    zr = nr;

    iteracoes++;
  } 
  return iteracoes;  
}

//*******************************************************************************//

void calcula_madelbrot(int *buffer, int l, int nx, int ny, double xmin, double xmax, double ymin, double ymax) {
  double delta_x, delta_y;
  double valor_x, valor_y;
  int x, y;

  delta_x = (xmax - xmin)/nx;
  delta_y = (ymax - ymin)/ny;

  for (y=l;  y<ny; y+=3){
    valor_y = ymin + delta_y * y;
    for (x=0; x<nx; x++) {
      valor_x = xmin + delta_x * x;
      buffer(y,x) = calcula_ponto(valor_x, valor_y);
    }
  }
}

//*******************************************************************************//
void gera_arquvio_ppm(char *nome_arquivo, int *buffer, int nx, int ny, int max) {
  int i;

  FILE *file = fopen(nome_arquivo,"w");

  fprintf(file,"P2\n");
  fprintf(file,"%d %d\n",nx,ny);

  fprintf(file,"%d",max);

  for (i=0; i<nx*ny; i++) {
    if (!(i%nx)){ 
	fprintf(file,"\n");
    }	
    fprintf(file,"%d ",buffer[i]);
  }
  fclose(file);
}

//*******************************************************************************//
int main( int argc,  char **argv ){
	int nx = 5000;
	int ny = 5000;
	key_t chave = 7;
	int shmid;
	int pid;

	int *buffer = NULL;	
	double xmin = -1.5;
	double ymin = -1.5;
	double xmax = 1.5;
	double ymax = 1.5;

	shmid = shmget(chave, sizeof(int)*nx*ny, 0600 | IPC_CREAT );

	buffer = shmat(shmid, 0 , 0 );
	
	pid = fork();
	
	if ( pid > 0 ){

		pid = fork();
		
		if ( pid > 0 ){

			calcula_madelbrot(buffer, 0, nx, ny, xmin, xmax, ymin, ymax);
			wait(NULL);
			wait(NULL);

			gera_arquvio_ppm("saida.ppm", buffer, nx, ny, 255);
	
			shmdt(buffer);
			shmctl(shmid,IPC_RMID,NULL);
		}
		else{
			calcula_madelbrot(buffer, 1, nx, ny, xmin, xmax, ymin, ymax);
			shmdt(buffer);
		}
	}
	else{
		calcula_madelbrot(buffer, 2, nx, ny, xmin, xmax, ymin, ymax);
		shmdt(buffer);
	}

}
//*******************************************************************************//
	
	




