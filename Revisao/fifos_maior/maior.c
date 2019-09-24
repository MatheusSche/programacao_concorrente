#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>


/*-------------------------------------------------------*/
int **aloca_matriz(int n, int m){
	int i;
	int **mat = (int **)malloc(sizeof(int *)*n);
	
	for (i=0;i<n;i++){
		mat[i] = (int *)malloc(sizeof(int)*m);
	}
	return(mat);
}

/*-------------------------------------------------------*/
void inicializa_matriz(int **mat, int n, int m){
	int i, j;
	
	srand(time(NULL));

	for( i=0; i<n; i++){
		for( j=0; j<m; j++){
			mat[i][j] = rand() % 100;;
		}
	}

}

/*-------------------------------------------------------*/
void escreve_matriz(int **mat, int n, int m){
	int i,j;
	
	for (i=0;i<n;i++){
		for (j=0;j<m;j++){
		    printf("% 5d ", mat[i][j]);
		}
		printf("\n");
	}
}


/*----------------------------------------------------------------------------*/
void desaloca_matriz(int **mat, int n){
	int i;
	
	for (i=0;i<n;i++){
		free(mat[i]);
	}
	free(mat);
}



/*-------------------------------------------------------*/
int maior(int ini, int pulo,int **mat, int n, int m){
	int i, j, maior;

	maior = mat[0][0];
 
	for (i=ini; i<n; i+=pulo){
		for (j=0; j<m; j++){
			if ( maior < mat[i][j]){
				maior = mat[i][j];
			}
		}
	}
	return(maior);
}

/*-------------------------------------------------------*/
int main(int argc, char **argv){

	int **mat;
	int n, m, aux;
	int pid, fd;
	char fifo[50] = "fifo";

	if ( argc != 2 ){
		printf("%s < ordem da matriz>\n", argv[0]);
		exit(0);
	}
	
	n = atoi(argv[1]);

	mat = aloca_matriz(n, n);

	inicializa_matriz(mat, n, n);

	escreve_matriz(mat, n, n);

	mkfifo(fifo,0666);

	pid = fork();

	if(pid > 0){
		fd = open(fifo, O_RDONLY);
		m = maior(1,2,mat, n, n);
		read(fd, &aux, sizeof(int));
		if(aux > m){
			m = aux;
		}
		printf("Maior: %d\n", m);
		unlink(fifo);
		
	} else {
		fd = open(fifo, O_WRONLY);
		m = maior(0,2,mat,n,n);
		write(fd, &m, sizeof(int));
		close(fd);
	}
	
	//;

	desaloca_matriz(mat, n);



}
	
/*-------------------------------------------------------*/
