#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/resource.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *args[]) {
    printf("--------------------- Child 0 Forked ---------------------\n");
    printf("-- Child 0 Task: Find count of \"t\"s in a string --\n\n");


    // sleep for provided amount of time
    sleep(atoi(args[1]));


    // count "t"s
    int8_t result_count = 0;
    for (uint8_t i = 0; i < strlen(args[2]); i++) {
        if (args[2][i] == 't') result_count++;
    }

    // child output
    printf("----- Child 0 Outputting -----\n");
    printf("  Child 0 PID: %d\n", getpid());
    printf("  Child 0 Parent PID: %d\n", getppid());
    printf("  Child 0 Resulting \"t\" count: %" PRIu8 "\n", result_count);

    // get resource usage info for child total user time
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    uint64_t cpu_time = (uint64_t)usage.ru_utime.tv_sec * 1000000 + (uint64_t)usage.ru_utime.tv_usec;

    // print cpu time in micro seconds
    printf("  Child 0 CPU time: %" PRIu64 " (micro secs)\n\n", cpu_time);

    return result_count;
}


