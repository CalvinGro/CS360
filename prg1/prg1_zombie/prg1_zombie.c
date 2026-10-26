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

        return 0;

    }
    
    // display processor status for child
    char sys_ps_cmd[40];
    printf("\nChild Status Info:\n");
    snprintf(sys_ps_cmd, sizeof(sys_ps_cmd), "ps -o pid,ppid,stat -p %d", child_pid);
    system(sys_ps_cmd);

    sleep(1);
    
    printf("\nThe child has turned into a zombie and now must be terminated.\n");
    wait(NULL);
    
    return 0;
}


