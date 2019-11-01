#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
/*-----------------------------------------*/
#define CHAIRS 5
#define CUSTOMERS 8
/*-----------------------------------------*/
struct argumentos{
    int id;
    int *waiting;
    sem_t *customers;
    sem_t *barbers;
    sem_t *mutex;
};
typedef struct argumentos ARGS;
/*-----------------------------------------*/
void * barbeiro(void *args){
    ARGS *p = (ARGS *)args;
    int *waiting = p->waiting;
    sem_t *customers = p->customers;
    sem_t *barbers = p->barbers;
    sem_t *mutex = p->mutex;
    int i;
    
    while ( 1 ){
        sem_wait(customers);
        sem_wait(mutex);
        (*waiting)--;
        sem_post(barbers);
        sem_post(mutex);
        printf("Cortando o Cabelo\n");
        sleep(5);
    }
    
}
/*-----------------------------------------*/
void * customer(void *args){
    ARGS *p = (ARGS *)args;
    int *waiting = p->waiting;
    int id = p->id;
    sem_t *customers = p->customers;
    sem_t *barbers = p->barbers;
    sem_t *mutex = p->mutex;
    int i;

    sem_wait(mutex);
    if((*waiting)<CHAIRS){
        (*waiting)++;
        sem_post(customers);
        sem_post(mutex);
        sem_wait(barbers);
        printf("Cliente [%d] esta cortando o cabelo\n", id);
    } else {
        sem_post(mutex);
        printf("Barbearia esta cheia. Vá embora\n");
    }
    sleep(2);
}
/*-----------------------------------------*/
int main(){
    pthread_t cid[CUSTOMERS];
    pthread_t bid;
    int i;

    sem_t customers, barbers, mutex;
    int waiting = 0;
    ARGS ab;
    ARGS ap[CUSTOMERS];

    sem_init(&customers, 0, 0);
    sem_init(&barbers, 0, 0);
    sem_init(&mutex, 0, 1);

    ab.barbers = &barbers;
    ab.customers = &customers;
    ab.waiting = &waiting;
    ab.mutex = &mutex;

    pthread_create(&bid, NULL, barbeiro, (void *)&ab);

    for(i=0;i<CUSTOMERS;i++){
        ap[i].id = i;
        ap[i].waiting = &waiting;
        ap[i].barbers = &barbers;
        ap[i].customers = &customers;
        ap[i].mutex = &mutex;
        pthread_create(&cid[i], NULL, customer,  (void *)&ap[i]);
    }

    pthread_join(bid, NULL);
    for(i=0;i<CUSTOMERS;i++){
        pthread_join(cid[i], NULL);
    }

    sem_destroy(&customers);
    sem_destroy(&barbers);
    sem_destroy(&mutex);
}
/*-----------------------------------------*/
/*-----------------------------------------*/
/*-----------------------------------------*/

