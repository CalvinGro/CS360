#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <string.h>
#include <sys/resource.h>

int main(int argc, char *args[]) {
    printf("----- Grandchild Outputting -----\n");
    printf("  Grandchild PID: %d\n", getpid());
    printf("  Grandchild Parent PID: %d\n", getppid());
    
    // get resource usage info for grandchild total user time
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    uint64_t cpu_time = (uint64_t)usage.ru_utime.tv_sec * 1000000 + (uint64_t)usage.ru_utime.tv_usec;

    // print cpu time in micro seconds
    printf("  Grandchild CPU time: %" PRIu64 "\n\n", cpu_time);

    return 0;
}