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

    msgid = msgget(chave, 0600 | IPC_CREAT);

    m.tipo = 1;
    strcpy(m.str, "ping");
    msgsnd(msgid, &m, sizeof(m.str), 0);
    /*tira a mensagem  da fila*/
    msgrcv(msgid, &m, sizeof(m.str), 2,0);

    printf("Retorno: %s\n", m.str);

    msgctl(msgid, IPC_RMID, 0);
}