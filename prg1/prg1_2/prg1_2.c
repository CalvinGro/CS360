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

    uint64_t total_cpu_time = 0;
    pid_t children_pids[CHILDREN_COUNT] = {0};
    struct rusage usage;

    printf("----------- Parent Started -----------\n\n");

    // open file
    FILE *shared_file = fopen("shared_doc.txt", "a");

    // loop through all children
    for (uint8_t i = 0; i < CHILDREN_COUNT; i++) {

        // create child if first loop o
        if (i == 0 || children_pids[i-1] != 0) {
            children_pids[i] = fork();
        }

        // if child 
        if (children_pids[i] == 0) {
            // get current childs time usage
            getrusage(RUSAGE_SELF, &usage);
            uint64_t cpu_time = (uint64_t)usage.ru_utime.tv_sec * 1000000 + (uint64_t)usage.ru_utime.tv_usec;

            // print first child msg to file
            fprintf(shared_file, "Hey this is child %" PRIu8 "\n My pid is: %d \nMy current cpu time is %" PRIu64 " (micro secs)\n\n", i, getpid(), cpu_time);
            fflush(shared_file);
            sleep(i);

            // print second child msg to file
            fprintf(shared_file, "This is child %" PRIu8 " again, I just waited %" PRIu8 " seconds.\n My parent's pid is: %d\n\n", i, i, getppid());
            fflush(shared_file);
            return 0;
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
        printf("  Child Actual time: %" PRIu64 " (micro secs)\n\n\n", child_time);
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
    printf("  Parent CPU time: %" PRIu64 " (micro secs)\n", parent_time);
    printf("  Total CPU time: %" PRIu64 " (micro secs)\n\n\n", total_cpu_time);
    

    // close file
    fclose(shared_file);

    return 0;
}


