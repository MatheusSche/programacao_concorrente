#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>	
#include <unistd.h>	
/*-------------------------------------------------------------------------*/
#define TAMMAX 200
	
/*-------------------------------------------------------------------------*/
void main(int argc, char **argv){
	
	FILE *out;
    int in;
	char buffer[TAMMAX];
	

	size_t bytesLidos;
		
	if ( argc != 2 ){
		printf("%s <arquivo_copia>\n", argv[0]);
		exit(0);
	}

	
  	out = fopen(argv[1], "w+");

    in = open("copia", O_RDONLY);
  	
	while((bytesLidos = read(in, buffer, TAMMAX)) ){ 
		 
  		fwrite(buffer, sizeof(char), bytesLidos, out);
		printf("%ld\n", bytesLidos);

  	}
  	
	close(in);
  	fclose(out);
}	
/*-------------------------------------------------------------------------*/
