#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/resource.h>

uint32_t main(char *args[]) {
    printf("--------------------- Child 0 Forked ---------------------\n");
    printf("-- Child 0 Task: Calculate the sum of integers 1 - 1000 --\n\n");


    // sleep for provided amount of time
    sleep(atoi(args[0]));

    // perform simple calculation
    uint32_t result_sum = 0;
    for (uint32_t i = 1; i <= 1000; i++) {
        result_sum += i;
    }

    // child output
    printf("----- Child 0 Output -----\n");
    printf("Child 0 PID: %d\n", getpid());
    printf("Parent PID: %d\n", getppid());
    printf("Resulting sum: %d\n", result_sum);


    // get resource usage info for child total user time
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    uint64_t cpu_time = (uint64_t)usage.ru_utime.tv_sec * 1'000'000 + (uint64_t)usage.ru_utime.tv_usec;

    // print cpu time in micro seconds
    printf("CPU Time: %" PRIu64 "\n\n\n", cpu_time);

    return result_sum;
}


