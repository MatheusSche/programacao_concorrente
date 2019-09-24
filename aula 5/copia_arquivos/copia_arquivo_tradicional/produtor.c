#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>	
/*-------------------------------------------------------------------------*/
#define TAMMAX 200
	
/*-------------------------------------------------------------------------*/
void main(int argc, char **argv){
	
	FILE *in;

	char buffer[TAMMAX];
	int out;

	size_t bytesLidos;
		
	if ( argc != 2 ){
		printf("%s <arquivo_original>\n", argv[0]);
		exit(0);
	}

    in  = fopen(argv[1], "r");
   
    mkfifo("copia", 0600);
    out = open("copia", O_WRONLY);
	
	while((bytesLidos = fread(buffer, sizeof(char), TAMMAX, in)) ){ 
		 
        write(out, buffer, bytesLidos);
  		
  	}
  	
	fclose(in);
    close(out);
    unlink("copia"); // remove
 
}	
/*-------------------------------------------------------------------------*/
