#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    
    int priority, pid;

    priority=atoi(argv[1]);
    pid=atoi(argv[2]);

    // if(0>priority || priority>100){
    //     printf("\nINVALID: Priority must be an integer between 0 and 100\n");
    //     exit(0);
    // }

    set_priority(priority, pid);

    // printf("Priority of process with pid %d has been changed to %d\n", pid, priority);

    exit(0);
}