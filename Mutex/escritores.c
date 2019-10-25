#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define L 3

/*---------------------------------------------------------*/
struct args{
    sem_t *db;
    sem_t *mutex;
    int *cont;
};
typedef struct args ARGS;

/*---------------------------------------------------------*/
void * leitor( void * args){
    ARGS *p = (ARGS *)args;
    sem_t *mutex = p->mutex;
    sem_t *db = p->db;
    int *cont = p->cont;

    while( 1 ){
        sem_wait(mutex);
        (*cont)++;
        if( *cont == 1){
            sem_wait(db);
        }
        sem_post(mutex);
        printf("Leitor Acessando: %d\n", *cont);
        sleep(10);

        sem_wait(mutex);
        (*cont)--;

        if(*cont == 0){
            sem_post(db);
        }
        sem_post(mutex);
        sleep(3);

    }
}

/*---------------------------------------------------------*/
void * escritor(void *args){
    ARGS *p = (ARGS *)args;
    sem_t *mutex = p->mutex;
    sem_t *db = p->db;
    int *cont = p->cont;

    while( 1 ){
        sem_wait(db);

        printf("Escriotor acessando\n");

        sem_post(db);

        sleep(5);
    }
}
/*---------------------------------------------------------*/




int main(){
    pthread_t te;
    pthread_t tl[L];
    int i;
    sem_t db, mutex;
    int cont = 0;
    ARGS a;

    sem_init(&db, 0, 1);
    sem_init(&mutex, 0, 1);

    a.db = &db;
    a.mutex = &mutex;
    a.cont = &cont;
    
    pthread_create(&te, NULL, escritor, (void *)&a);

    for(i=0;i<L;i++){
        pthread_create(&tl[i], NULL, leitor, (void *)&a);
    }

    pthread_join(te, NULL);

    for(i=0; i<L; i++){
        pthread_join(tl[i], NULL);
    }

    sem_destroy(&db);
    sem_destroy(&mutex);

}
/*---------------------------------------------------------*/