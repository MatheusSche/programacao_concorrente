#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define N 20

/*---------------------------------------------------------*/
struct args{
    int *contaPostIt;
    sem_t *mutex;
    sem_t *cheia;
    sem_t *enchendo;
};
typedef struct args ARGS;

/*---------------------------------------------------------*/
void * usuario( void * args){
    ARGS *p = (ARGS *)args;
    int *contaPostIt = p->contaPostIt;
    sem_t *mutex = p->mutex;
    sem_t *cheia = p->cheia;
    sem_t *enchendo = p->enchendo;

    while( 1 ){
        sem_wait(enchendo);
        sem_wait(mutex);
        //cola o post it na mochilo do pombo
        printf("Cola Post It na mochila do pombo: [%d]\n", (*contaPostIt));
        (*contaPostIt)++;
        
        if( *contaPostIt == N){
            sem_post(cheia);
        }
        sem_post(mutex);
        

    }
}

/*---------------------------------------------------------*/
void * pombo(void *args){
    ARGS *p = (ARGS *)args;
    int *contaPostIt = p->contaPostIt;
    sem_t *mutex = p->mutex;
    sem_t *cheia = p->cheia;
    sem_t *enchendo = p->enchendo;

    while( 1 ){
        sem_wait(cheia);
        sem_wait(mutex);
        //leva a mochila o pombo
        printf("Pombo leva a mochila: [%d]", (*contaPostIt));
        sleep(5);
        (*contaPostIt) = 0;

        for(int i=0; i<N; i++){
            sem_post(enchendo);
        }
        
        sem_post(mutex);
        
    }
}
/*---------------------------------------------------------*/


int main(){
    pthread_t te;
    pthread_t tl[N];
    int i;
    sem_t mutex, cheia, enchendo;
    int contaPostIt = 0;
    ARGS a;

    
    sem_init(&mutex, 0, 1);
    sem_init(&cheia, 0, 0);
    sem_init(&enchendo, 0, N);

    a.mutex = &mutex;
    a.cheia = &cheia;
    a.enchendo = &enchendo;
    a.contaPostIt = &contaPostIt;
    
    pthread_create(&te, NULL, pombo, (void *)&a);

    for(i=0;i<N;i++){
        pthread_create(&tl[i], NULL, usuario, (void *)&a);
    }

    pthread_join(te, NULL);

    for(i=0; i<N; i++){
        pthread_join(tl[i], NULL);
    }

    sem_destroy(&cheia);
    sem_destroy(&mutex);
    sem_destroy(&enchendo);

}
/*---------------------------------------------------------*/