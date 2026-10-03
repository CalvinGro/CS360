#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>  
#include <sys/resource.h>


int main(int argc, char *args[]) {
    printf("--------------------- Child 1 Forked ---------------------\n");
    printf("-- Child 1 Task: Create 2 grandchildren --\n\n");


    // sleep for provided amount of time
    sleep(atoi(args[1]));

    // create 2 grandchildren
    pid_t grdchd_0 = 0, grdchd_1 = 0;
    grdchd_0 = fork();
    if (grdchd_0 != 0) {
        grdchd_1 = fork();
    }

    // if grandchild, execute grandchild process
    char *grdchd_args[2] = {"grandchild", NULL};
    if (grdchd_1 == 0) execvp("./grandchild", grdchd_args);
    
    // wait for grandchildren to die
    wait(NULL);
    wait(NULL);

    // count grandchildren
    int8_t grdchd_count = 0;
    if (grdchd_0 != 0) grdchd_count++;
    if (grdchd_1 != 0) grdchd_count++;

    // child output
    printf("----- Child 1 Outputting -----\n");
    printf("  Child 1 PID: %d\n", getpid());
    printf("  Child 1 Parent PID: %d\n", getppid());
    printf("  Child 1 Grandchildren count: %" PRIu8 "\n", grdchd_count);

    // get resource usage info for child total user time
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    uint64_t cpu_time = (uint64_t)usage.ru_utime.tv_sec * 1000000 + (uint64_t)usage.ru_utime.tv_usec;

    // print cpu time in micro seconds
    printf("  Child 1 CPU time: %" PRIu64 " (micro sec)\n\n", cpu_time);

    return grdchd_count;
}