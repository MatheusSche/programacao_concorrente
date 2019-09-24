#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>

int main(){
    int shmid;
    int chave = 6;
    char *str = NULL;
    int pid, i;

    shmid = shmget(chave, 27 * sizeof(char), 0600 | IPC_CREAT);

    str = shmat(shmid,0,0);

    pid = fork();

    if (pid > 0){
        wait();
        printf("Alfabeto: [%s] \n", str);
        shmdt(str);
        shmctl(shmid, IPC_RMID, 0);
    } else {
        for(i=0; i<26; i++){
            str[i] = 'A'+i;
        }
        str[i] = '\0';
        shmdt(str);
    }
}