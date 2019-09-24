#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main(){
    int id;


    id = fork();

    if (id > 0){
        /*printf("id=%d pid=%d\n", id, getpid());*/
        sleep(5);
        kill(id, SIGSTOP);
        sleep(5);
        kill(id, SIGCONT);
        sleep(5);
        kill(id, SIGKILL);
    } else {
        while(1){
            printf("*");
        }
    }
}