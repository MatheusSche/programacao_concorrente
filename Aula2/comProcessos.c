#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main (){
    char msg[100];
    int id; 
    int p[2];
    
    //cria o pipe, vetor de duas posições para comunicação entre processos
    pipe(p);
    //systemcall para criação de processos   
    id = fork();

    if(id > 0){
        printf("Sou o pai - id = %d  pid=%d\n", id, getpid());
        //fecha uma posição do pipe, fecha o canal de enviar dados (escrita)
        close(p[1]);
        read(p[0], msg, 100);
        printf("Recebi: %s\n", msg);
        close(p[0]);
    } else {
        printf("Sou o filho - id = %d  pid=%d\n", id, getpid());
        //fecha uma posição do pipe, fecha o canal de receber dados (leitura)
        close(p[0]);
        strcpy(msg, "Pipe - Alo Mundo\n");
        write(p[1], msg, strlen(msg)+1);
        //(onde vou gravar, o que eu vou enviar, quantos bytes vou enviar,)
        close(p[1]);
    }
}