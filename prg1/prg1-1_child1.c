#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/resource.h>

uint32_t main(char *args[]) {
    printf("--------------------- Child 1 Forked ---------------------\n");
    printf("-- Child 1 Task: Create 2 grandchildren --\n\n");


    // sleep for provided amount of time
    sleep(atoi(args[0]));

    // create 2 grand children

    // child output
    printf("----- Child 1 Output -----\n");
    printf("Child 1 PID: %d\n", getpid());
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