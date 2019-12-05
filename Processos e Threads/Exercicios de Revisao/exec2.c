#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

/*--------------------------------------------------------------*/
float SalarioBruto, Horas, ValorHora, INSS, IR, Rend;
float Rendimentos, HorasExtras, Desconto, SalarioLiquido;
/*--------------------------------------------------------------*/
sem_t s0, s1,s2,s3;
/*--------------------------------------------------------------*/
void * fluxo1(void *args){
    SalarioBruto = Horas * ValorHora;
    sem_post(&s0);
    sem_post(&s1);
    INSS = SalarioBruto * 0.11;
    sem_wait(&s2);
    Desconto = IR + INSS;
    sem_post(&s3);
}
/*--------------------------------------------------------------*/
void * fluxo2(void *args){
    sem_wait(&s0);
    IR = SalarioBruto * 0.25;
    sem_post(&s2);
}
/*--------------------------------------------------------------*/


/*--------------------------------------------------------------*/
// valor zero para a thread ficar esperando no semaforo
int main(){
    
    pthread_t f1, f2;

    Horas = 100;
    ValorHora = 10;
    HorasExtras = 10;

    sem_init(&s0, 0,0);
    sem_init(&s1, 0,0);
    sem_init(&s2, 0,0);
    sem_init(&s3, 0,0);

    pthread_create(&f1, NULL, fluxo1, NULL);
    pthread_create(&f2, NULL, fluxo2, NULL);

    Rendimentos = HorasExtras * ValorHora;
    sem_wait(&s1);
    Rend = SalarioBruto + Rendimentos;
    sem_wait(&s3);
    SalarioLiquido = Rend - Desconto;

    pthread_join(f1, NULL);
    pthread_join(f2, NULL);
    printf("Salario Liquido: [%.2f]\n", SalarioLiquido);

    sem_destroy(&s0);
    sem_destroy(&s1);
    sem_destroy(&s2);
    sem_destroy(&s3);


}
/*--------------------------------------------------------------*/