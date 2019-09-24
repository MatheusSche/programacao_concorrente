#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>	
#include <sys/stat.h>

/*-------------------------------------------------------------------------*/
#define TAMMAX 200
	
/*-------------------------------------------------------------------------*/
void main(int argc, char **argv){
	
	FILE *in;

	char buffer[TAMMAX];
	int out;

	size_t bytesLidos;
		
	if ( argc != 3 ){
		printf("%s <arquivo_original> <fifo>\n", argv[0]);
		exit(0);
	}
	
	in  = fopen(argv[1], "rb");

	mkfifo(argv[2], 0600);
	out = open(argv[2], O_WRONLY);
	  	

	while((bytesLidos = fread(buffer, sizeof(char), TAMMAX, in)) ){ 
  		write(out, buffer, bytesLidos);
 	}
  	
	fclose(in);
  	
	close(out);
	unlink(argv[2]);

}	
/*-------------------------------------------------------------------------*/
