#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct parametros{
    int id;
    long npontos;
    int nthreads;
    long cont;
};
typedef struct parametros PAR;

void * funcao(void *args){
    PAR *p = (PAR *)args;
    int id = p->id;
    long npontos = p->npontos;
    int nthr = p->nthreads;
    long i, cont = 0;
    double x, y, distancia;
    
    //printf("ID: %d!!\n", p->id);
    //printf("NPONTOS: %ld!!\n", (*p).npontos);
    //printf("NTHR: %d!!\n", (*p).nthreads);
    //int *p = (int *)args;
    //printf("Alo pthreads: %d\n", *p);

    int seed = id;
    //srand(time(NULL));//n]ao é thread safe
    for(i=0;i<npontos/nthr;i++){
        x = rand_r(&seed)/(double)RAND_MAX;
        y = rand_r(&seed)/(double)RAND_MAX;
        distancia = sqrt(x*x + y*y);

        if(distancia<=1){
            cont++;
        }
    }

    p->cont = cont;

}

int main(int argc, char **argv){
    
    int nt, i;
    PAR *p;
    long npontos;
    long cont;
    double pi;
    
    if (argc != 3){
        printf("%s <num_pontos> <num_threads>\n", argv[0]);
        exit(0);
    }

    npontos = atol(argv[1]);
    nt = atoi(argv[2]);

    pthread_t *tid = (pthread_t *) malloc(nt * sizeof(pthread_t));

    p = (PAR *)malloc(nt*sizeof(PAR));
    
    for (i=0;i<nt;i++){
        p[i].id = i;
        p[i].npontos = npontos;
        p[i].nthreads = nt;
        p[i].cont = 0;
        
        pthread_create(&tid[i], NULL, funcao, (void *)&p[i]);
    }

    cont = 0;
    for(i=0;i<nt;i++){
        pthread_join(tid[i], NULL);
        cont+=p[i].cont;
    }

    pi = 4 * (double)cont/npontos;
    printf("PI: %.15f\n", pi);

    free(tid);
    free(p);
    
    //pthread_create(&tid, NULL, funcao, NULL);

    //pthread_join(tid, NULL);
    //ESPERA POR (IDENTIFICADOR DA THREAD, RECEBE PONTEIRO DE ENDEREÇO DE RETORNO)
}