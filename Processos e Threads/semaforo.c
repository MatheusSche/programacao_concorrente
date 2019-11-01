#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
	
/*-------------------------------------------------------------------------*/
#define TAMMAX 200

/*-------------------------------------------------------------------------*/
struct parametros{
	FILE *f;
	char *buffer;
	int *bytesLidos;
	sem_t *full;
	sem_t *empty;

};
typedef struct parametros PARAMETROS;
/*-------------------------------------------------------------------------*/
void * le_arquivo( void * args ){
	PARAMETROS *p = (PARAMETROS *)args;
	FILE *in = p->f;
	char *buffer = p->buffer;
	int *bytesLidos = p->bytesLidos;
	sem_t *full = p->full;
	sem_t *empty = p->empty;
	
	do{
		sem_wait(empty);
		*bytesLidos = fread(buffer, sizeof(char), TAMMAX, in);
		sem_post(full);

	} while (*bytesLidos);	
}

/*-------------------------------------------------------------------------*/
void * escreve_arquivo( void * args ){
	PARAMETROS *p = (PARAMETROS *)args;
	FILE *out = p->f;
	char *buffer = p->buffer;
	int *bytesLidos = p->bytesLidos;
	sem_t *full = p->full;
	sem_t *empty = p->empty;
	do{
		sem_wait(full);
  		fwrite(buffer, sizeof(char), *bytesLidos, out);
		  sem_post(empty);	
	}
	while(*bytesLidos);

	;
}

/*-------------------------------------------------------------------------*/
void main(int argc, char **argv){ 
	
	FILE *in, *out;
	char buffer[TAMMAX];
	int bytesLidos;
	
	sem_t full;
	sem_t empty;

	pthread_t te, tl;
	PARAMETROS pe, pl;

	if ( argc != 3 ){
		printf("%s <arquivo_original> <arquivo_copia>\n", argv[0]);
		exit(0);
	}

	in  = fopen(argv[1], "r");
  	out = fopen(argv[2], "w+");

	sem_init(&full,0,0);
	sem_init(&empty,0,1);

	pl.f = in;
	pl.buffer = buffer;
	pl.bytesLidos = &bytesLidos;
	pl.full = &full;
	pl.empty = &empty;
	pthread_create(&tl, NULL, le_arquivo, (void *)&pl);
	
	pe.f = out;	
	pe.buffer = buffer;
	pe.bytesLidos = &bytesLidos;
	pe.full = &full;
	pe.empty = &empty;
	pthread_create(&te, NULL, escreve_arquivo, (void *)&pe);
	
	pthread_join(tl, NULL);
	pthread_join(te, NULL);

	sem_destroy(&full);
	sem_destroy(&empty);

	fclose(in);
  	fclose(out);
}	
/*-------------------------------------------------------------------------*/
