#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#define T 5

/*--------------------------------------------------------------*/
sem_t barreira;
sem_t mutex;
int cont;


/*--------------------------------------------------------------*/
void * thread(void *args){
    int id = (int)(long)args;

    printf("Entrando Thread %d!!\n", id);

    sleep(id);

    sem_wait(&mutex);
    cont++;
    if(cont<T){
        sem_post(&mutex);
        sem_wait(&barreira);
    } else {
        cont = 0;
        for(int i=0; i<T-1;i++){
            sem_post(&barreira);
        }
        sem_post(&mutex);
    }


    printf("Saindo Thread %d!!\n", id);
}
/*--------------------------------------------------------------*/




/*--------------------------------------------------------------*/
int main(){
    int i;

    sem_init(&mutex,0,1);
    sem_init(&barreira,0,0);

    pthread_t tid[T];

    for(i=0; i<T; i++){
        pthread_create(&tid[i], NULL, thread, (void *)(long)i);
    }

    for(i=0; i<T; i++){
        pthread_join(tid[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&barreira);

}
/*--------------------------------------------------------------*/