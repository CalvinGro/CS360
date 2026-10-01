#include <stdio.h>
#include <stdint.h>
#include <unistd.h>


int main(void) {

    uint8_t child_sleep_times[4] = {3,5,7,6};


    for (uint8_t i = 0; i < 4; i++) {
        uint8_t cur_sleep_time = child_sleep_times[i % 4];

        pid_t pid = fork();

        // if child 
        if (pid == 0) {
            // create child name prg1-1_child(n).c
            char child_prg_name[20];
            snprintf(child_prg_name, sizeof(child_prg_name), "./prg1-1_child%d", i % 4);

            char *args[] = {cur_sleep_time};

            execvp(child_prg_name, args);
        }
    }
    wait(NULL);
    

    return 0;
}


