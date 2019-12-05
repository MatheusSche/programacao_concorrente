#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define L 3

/*---------------------------------------------------------*/
int a=0, b=0, c=0, d=0;
sem_t sem1, sem2;

/*---------------------------------------------------------*/
void * expressao( void * args){
    c = 3;
    
    sem_post(&sem1);
    sem_wait(&sem2);
    a = a + d;

    sem_post(&sem1);
}
/*---------------------------------------------------------*/

int main(){
    int i;
    pthread_t th;

    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);

    pthread_create(&th, NULL, expressao, NULL);
    
    a = 1;
    sem_wait(&sem1);

    b = a + c;

    d = b;
    sem_post(&sem2);
    sem_wait(&sem1);

    c = b * d;

    printf("C: [%d]", c);
    
    pthread_join(th, NULL);

    sem_destroy(&sem1);
    sem_destroy(&sem2);

}
/*---------------------------------------------------------*/