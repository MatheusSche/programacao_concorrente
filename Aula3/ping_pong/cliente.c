#include <sys/msg.h>
#include <stdio.h>
#include <string.h>

/*-------------------------------------------*/
struct msg{
    long tipo;
    char str[100];
};
typedef struct msg MSG;
/*-------------------------------------------*/



int main(){
    int msgid;
    int chave = 5;
    MSG m;

    msgid = msgget(chave, 0);

   
    /*tira a mensagem  da fila*/
    msgrcv(msgid, &m, sizeof(m.str), 1,0);

    m.tipo = 2;
    strcat(m.str, "-pong");

    msgsnd(msgid, &m, sizeof(m.str), 0);

    
}