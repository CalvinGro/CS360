#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>  
#include <sys/resource.h>

#define CHILDREN_COUNT 4

int main(void) {

    uint8_t child_sleep_times[CHILDREN_COUNT] = {3,5,7,6};
    uint64_t total_cpu_time = 0;
    pid_t children_pids[CHILDREN_COUNT] = {0};

    printf("----------- Parent Started -----------\n\n");

    // loop through all children and assign them their sleep times
    for (uint8_t i = 0; i < CHILDREN_COUNT; i++) {
        char cur_sleep_time[4];
        snprintf(cur_sleep_time, sizeof(cur_sleep_time), "%" PRIu8, child_sleep_times[i % CHILDREN_COUNT]);


        // create child
        children_pids[i] = fork();

        // if child 
        if (children_pids[i] == 0) {
            // create child name prg1-1_child(n).c
            char child_prg_name[20];
            snprintf(child_prg_name, sizeof(child_prg_name), "./child%d", i % CHILDREN_COUNT);

            // NULL is needed at the end of args to prevent errors
            char *args[4] = {child_prg_name, cur_sleep_time, NULL, NULL};

            // if child 2, pass in array to find largest element from it
            if (2 == i % CHILDREN_COUNT) {
                printf("Child 2 array = [2 4 3 65 3 55 6 35 46 34 23 1 2 37 67 0 4 2]\n\n");
                args[2] = "2 4 3 65 3 55 6 35 46 34 23 1 2 37 67 0 4 2";
            }

            // if child 0 or 3, pass in a string
            if (0 == i || 3 == i) {
                printf("Child %" PRIu8 " sentence: A fat cat sat by the east window and watched the birds.\n\n", i);
                args[2] = "A fat cat sat by the east window and watched the birds.";
            }

            execvp(child_prg_name, args);
            printf("Child %" PRIu8 " failed\n", i);
        }
    }

 
    // wait for children to finish in order
    for (uint8_t i = 0; i < CHILDREN_COUNT; i++) {
        int child_status; 
        struct rusage child_usage;
        wait4(children_pids[i], &child_status, 0, &child_usage);
        uint8_t child_output = WEXITSTATUS(child_status);
        uint64_t child_time = (uint64_t)child_usage.ru_utime.tv_sec * 1000000 + (uint64_t)child_usage.ru_utime.tv_usec;

        total_cpu_time += child_time;

        printf("----- Child %" PRIu8 " Terminated -----\n", i);
        printf("  Child Result: %" PRIu8 "\n", child_output);
        printf("  Child Actual time: %" PRIu64 " (micro sec)\n\n\n", child_time);
    }

    // start parent output
    printf("----- Parent Output -----\n");
    printf("  Parent's own PID: %d\n", getpid());

    // print children pids
    for (uint8_t i = 0; i < CHILDREN_COUNT; i++) {
        printf("  Child %" PRIu8 " PID: %d\n", i, children_pids[i]);
    }

    // get parent cpu time
    struct rusage parent_usage;
    getrusage(RUSAGE_SELF, &parent_usage);
    uint64_t parent_time = (uint64_t)parent_usage.ru_utime.tv_sec * 1000000 + (uint64_t)parent_usage.ru_utime.tv_usec;
    total_cpu_time += parent_time;

    // print cpu info
    printf("  Parent CPU time: %" PRIu64 " (micro sec)\n", parent_time);
    printf("  Total CPU time: %" PRIu64 " (micro sec)\n\n\n", total_cpu_time);
    

    return 0;
}


