#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/resource.h>
#include <string.h>

int main(int argc, char *args[]) {
    printf("--------------------- Child 2 Forked ---------------------\n");
    printf("-- Child 2 Task: Find largest integer from array arg --\n\n");

    // sleep for provided amount of time
    sleep(atoi(args[1]));

    // turn arg back into array
    int8_t arr_ints[20] = {0};
    int8_t i = 0;
    for (char *num = strtok(args[2], " "); num != NULL; num = strtok(NULL, " ")) {
        arr_ints[i] = (int8_t)atoi(num);
        i++;
    }

    // search through array of ints for max element
    int8_t result_largest = 0;
    for (int8_t j = 0; j < sizeof(arr_ints); j++) {
        if (arr_ints[j] > result_largest) {
            result_largest = arr_ints[j];
        }
    }

    // child output
    printf("----- Child 2 Outputting -----\n");
    printf("  Child 2 PID: %d\n", getpid());
    printf("  Child 2 Parent PID: %d\n", getppid());
    printf("  Child 2 Resulting largest int: %" PRIu8 "\n", result_largest);

    // get resource usage info for child total user time
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    uint64_t cpu_time = (uint64_t)usage.ru_utime.tv_sec * 1000000 + (uint64_t)usage.ru_utime.tv_usec;

    // print cpu time in micro seconds
    printf("  Child 2 CPU time: %" PRIu64 " (micro sec)\n\n", cpu_time);

    return result_largest;
}
