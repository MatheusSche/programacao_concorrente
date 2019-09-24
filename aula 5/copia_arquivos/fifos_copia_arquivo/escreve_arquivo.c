#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>	
#include <sys/stat.h>

/*-------------------------------------------------------------------------*/
#define TAMMAX 200
	
/*-------------------------------------------------------------------------*/
void main(int argc, char **argv){
	
	FILE *out;

	char buffer[TAMMAX];
	int in;

	size_t bytesLidos;
		
	if ( argc != 3 ){
		printf("%s <arquivo_copia> <fifo>\n", argv[0]);
		exit(0);
	}
	
	out  = fopen(argv[1], "wb");

	in = open(argv[2], O_RDONLY);

	while((bytesLidos = read(in, buffer, TAMMAX)) ){ 
  		fwrite(buffer, sizeof(char), bytesLidos, out);
		printf("%d\n", bytesLidos);
  	}
  	
	fclose(out);
  	close(in);
}	
/*-------------------------------------------------------------------------*/
