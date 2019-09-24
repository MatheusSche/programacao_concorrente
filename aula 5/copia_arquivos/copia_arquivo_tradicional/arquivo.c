#include <stdio.h>
#include <stdlib.h>
	
/*-------------------------------------------------------------------------*/
#define TAMMAX 200
	
/*-------------------------------------------------------------------------*/
void main(int argc, char **argv){
	
	FILE *in, *out;

	char buffer[TAMMAX];
	

	size_t bytesLidos;
		
	if ( argc != 3 ){
		printf("%s <arquivo_original> <arquivo_copia>\n", argv[0]);
		exit(0);
	}

	
	in  = fopen(argv[1], "r");
  	out = fopen(argv[2], "w+");
  	
	while((bytesLidos = fread(buffer, sizeof(char), TAMMAX, in)) ){ 
		 
  		fwrite(buffer, sizeof(char), bytesLidos, out);
		printf("%d\n", bytesLidos);
  	}
  	
	fclose(in);
  	fclose(out);
}	
/*-------------------------------------------------------------------------*/
