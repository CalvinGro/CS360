#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>  
#include <sys/resource.h>
#include <sys/socket.h>

#define HELPER_COUNT 2

int main(void) {
    pid_t helper_pids[HELPER_COUNT] = {0};


    // create P3 and P4 helpers
    for (pid_t i = 0; i < HELPER_COUNT; i++) {

        char p_num[3];
        snprintf(p_num, sizeof(p_num), "%d", i + 3);

        helper_pids[i] = fork();
        if (helper_pids[i] == 0) {
            char* args[3] = {"./helper", p_num, NULL};
            execvp("./helper", args);
            return 1;
        }
    }

    // create parent <-> child socket pair
    int sv[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1) {
        printf("Socket Pair setup failed\n");
    }

    // create child
    pid_t child_pid = 0;
    child_pid = fork();

    // child process
    if (child_pid == 0) {
        close(sv[0]);

        char sock_arg[40];
        snprintf(sock_arg, sizeof(sock_arg), "%d", sv[1]);

        // NULL is needed at the end of args to prevent errors
        char *args[3] = {"./child", sock_arg, NULL};
        execvp("./child", args);

    // parent process
    } else {
        close(sv[1]);

        // accept filename
        char input_file_name[60];
        char file_name[64];
        scanf("%59s", input_file_name);
        
        // create file and print to file
        snprintf(file_name, sizeof(file_name), "%s.txt", input_file_name);
        FILE* shared_file = fopen(file_name, "a");

        fprintf(shared_file, "-Parent- Parent %d writing to file\n", getpid());
        fflush(shared_file);

        // pass file name to child
        // passing a file pointer requires a very intensive store alignment and file
        // control passing processes.
        write(sv[0], file_name, strlen(file_name)+1);
        
        // receive msg from child and close socket
        char child_msg[100];
        read(sv[0], child_msg, sizeof(child_msg));
        close(sv[0]);


        // write child's msg to file
        fprintf(shared_file, "\n-Parent- The child's final msg was: \n%s\n", child_msg);

        // detect child's death and close file
        waitpid(child_pid, NULL, 0);
        
        fclose(shared_file);

        // show file
        printf("-Parent- Shared .txt File Content: \n");
        fflush(stdout);

        char sys_call[100];
        snprintf(sys_call, sizeof(sys_call), "cat %s", file_name);
        system(sys_call);
        
        
        waitpid(helper_pids[0], NULL, 0);
        waitpid(helper_pids[1], NULL, 0);
        
        return 0;
    }
}