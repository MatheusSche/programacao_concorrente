#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>

struct parametros{
    int *temp;    
    sem_t *sts, *st1, *st2;
};

typedef struct parametros PARAMETROS;

void  *le_sensor(void *args){
    
    PARAMETROS *p = (PARAMETROS *)args;
    int *temp = p->temp;
    sem_t *st1 = p->st1;
    sem_t *st2 = p->st2;
    sem_t *sts = p->sts;

    while(1){
        sem_wait(sts);
        *temp = rand()%100;
        if(*temp<50){
            sem_post(st1);
        } else {
            sem_post(st2);
        }
    }
}

void *tratador1(void *args){
    PARAMETROS *p = (PARAMETROS *)args;
    int *temp = p->temp;
    sem_t *sts = p->sts;
    sem_t *st1 = p->st1;
    sem_t *st2 = p->st2;

    while(1){
        sem_wait(st1);
        printf("Tratador 1 %d\n", *temp);
        fflush(stdout);
        sleep(10);
        sem_post(sts);
    }
    
}

void *tratador2(void *args){
    PARAMETROS *p = (PARAMETROS *)args;
    int *temp = p->temp;
    sem_t *sts = p->sts;
    sem_t *st1 = p->st1;
    sem_t *st2 = p->st2;

    while(1){
        sem_wait(st2);
        printf("Tratador 2 %d\n", *temp);
        fflush(stdout);
        sleep(10);
        sem_post(sts);
    }
    
}

/*-------------------------------------------------------------------------*/
void main(int argc, char **argv){ 
		
	sem_t sts, st1, st2;
    int temp;
	
    pthread_t ts, t1, t2;
	PARAMETROS params;

	sem_init(&sts,0,1);
	sem_init(&st1,0,0);
    sem_init(&st2,0,0);

	
    params.temp = &temp;
    params.sts = &sts;
    params.st1 = &st1;
    params.st2 = &st2;
    
    pthread_create(&ts, NULL, le_sensor, (void *)&params);
	
	pthread_create(&t1, NULL, tratador1, (void *)&params);
	
    pthread_create(&t2, NULL, tratador2, (void *)&params);

	pthread_join(ts, NULL);
	pthread_join(t1, NULL);
    pthread_join(t2, NULL);

}	
/*-------------------------------------------------------------------------*/