#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

/*--------------------------------------------------------------*/
#define L 6
#define C 6
#define T 3
/*--------------------------------------------------------------*/
int mat[L][C];
/*--------------------------------------------------------------*/
sem_t s[T];
/*--------------------------------------------------------------*/
void * inicializa(void *args){
    int *p = (int *)args;
    int id = *p;
    
    printf("Sou uma thread: %d\n", id);
    
    int i, j;
    
    for(i=id;i<L; i+=T){
        for (j=0; j < C; j++){
            mat[i][j] = 1;
        }
        
    }

    sem_post(&s[id]);
    
}
/*--------------------------------------------------------------*/
void escreve(){
    int i, j;
    
    for(i=0;i<L; i++){
        for (j=0; j < C; j++){
            printf("%5d", mat[i][j]);
        }
        printf("\n");
    }
    
}
/*--------------------------------------------------------------*/


/*--------------------------------------------------------------*/
int main(){
    int i;
    pthread_t tid[T];
    int id[T];

    for(i=0; i<T; i++){
        sem_init(&s[i],0,0);
    }
    
    for(i=0;i<T;i++){
        id[i] = i;
        pthread_create(&tid[i], NULL, inicializa, (void *)&id[i]);
    }

    for(i=0; i<T; i++){
        sem_wait(&s[i]);
    }

    for(i=0; i<T; i++){
        sem_destroy(&s[i]);
    }
     
    escreve();

}
/*--------------------------------------------------------------*/