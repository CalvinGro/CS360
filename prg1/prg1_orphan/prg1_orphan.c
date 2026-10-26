#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/wait.h>  
#include <unistd.h>
#include <stdlib.h>

int main(void) {

    pid_t child_pid = 0;
    pid_t parent_pid = getpid();

    child_pid = fork();

    if (child_pid == 0) {

        sleep(1);

        child_pid = getpid();

        // display processor status
        char sys_ps_cmd[40];
        printf("\nChild Status Info:\n");
        snprintf(sys_ps_cmd, sizeof(sys_ps_cmd), "ps -o pid,ppid,stat -p %d", child_pid);
        system(sys_ps_cmd);

        printf("\n\nThis is an orphaned child.\nMy pid is: %d\n", child_pid);
        printf("My parent's pid was: %d\nMy parent's pid is now: %d\n", parent_pid, getppid());
    } else {
        printf("\nThis is the parent, my pid is: %d \nI am now terminating.\n", parent_pid);
    }

    
    return 0;
}


