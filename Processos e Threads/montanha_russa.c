#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
/*-----------------------------------------*/
#define P 10
/*-----------------------------------------*/
struct argumentos{
    int id;
    int *npass;
    sem_t *passageiro;
    sem_t *carrinho;
    sem_t *andando;
    sem_t *mutex;
};
typedef struct argumentos ARGS;
/*-----------------------------------------*/
void * funcao_passageiro(void *args){
    ARGS *p = (ARGS *)args;
    int *npass = p->npass;
    int id = p->id;
    sem_t *passageiro = p->passageiro;
    sem_t *carrinho = p->carrinho;
    sem_t *andando = p->andando;
    sem_t *mutex = p->mutex;
    //printf("Sou o passageiro [%d]\n", id);
    while(1){
        sleep(rand()%5+1);
        sem_wait(passageiro);
        printf("Passageiro [%d] entrando no carrinho\n", id);
        sem_wait(mutex);
        (*npass)++;
        if(*npass == P){
            sem_post(carrinho);
            sem_wait(andando);
            sem_post(mutex);
        } else{
            sem_post(mutex);
            sem_wait(andando);
        }
    }
}

/*-----------------------------------------*/
void * funcao_carrinho(void *args){
    ARGS *p = (ARGS *)args;
    int *npass = p->npass;
    sem_t *passageiro = p->passageiro;
    sem_t *carrinho = p->carrinho;
    sem_t *andando = p->andando;
    sem_t *mutex = p->mutex;
    int i;

    while(1){
        sem_wait(carrinho);
        *npass=0;
        for(i=0;i<P;i++){
            sem_post(andando);
            sem_post(passageiro);
        }
        printf("Carrinho partindo\n");
        sleep(5);
    }
    
}

int main(){
    pthread_t pid[P];
    pthread_t cid;
    int i;

    sem_t passageiro, carrinho, andando, mutex;
    int npass = 0;
    ARGS ac;
    ARGS ap[P];

    sem_init(&passageiro, 0, P);
    sem_init(&carrinho, 0, 0);
    sem_init(&andando, 0, 0);
    sem_init(&mutex, 0, 1);

    ac.npass = &npass;
    ac.passageiro = &passageiro;
    ac.carrinho = &carrinho;
    ac.andando = &andando;
    ac.mutex = &mutex;
    
    pthread_create(&cid, NULL, funcao_carrinho, (void *)&ac);

    for(i=0;i<P;i++){
        ap[i].id = i;
        ap[i].npass = &npass;
        ap[i].passageiro = &passageiro;
        ap[i].carrinho = &carrinho;
        ap[i].andando = &andando;
        ap[i].mutex = &mutex;
        pthread_create(&pid[i], NULL, funcao_passageiro, (void *)&ap[i]);
    }



    pthread_join(cid, NULL);
    for(i=0;i<P;i++){
        pthread_join(pid[i], NULL);
    }

    sem_destroy(&passageiro);
    sem_destroy(&carrinho);
    sem_destroy(&andando);
    sem_destroy(&mutex);
}