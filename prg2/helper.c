#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>


int main(int argc, char *args[]) {

    int p_num = (int)strtol(args[1], NULL, 10);

    char receive_pipe[20];
    snprintf(receive_pipe, sizeof(receive_pipe), "/tmp/fifo_to_p%d", p_num);
    char send_pipe[20];
    snprintf(send_pipe, sizeof(send_pipe), "/tmp/fifo_from_p%d", p_num);

    // create named pipe for child -> pi (if not already created)
    if (mkfifo(receive_pipe, 0666) == -1) {
        if (errno != EEXIST) return 1;
    }

    // create named pipe for child <- p3 (if not already created)
    if (mkfifo(send_pipe, 0666) == -1) {
        if (errno != EEXIST) return 1;
    }

    // open fds
    int fd_from_child = open(receive_pipe, O_RDONLY);
    int fd_to_child = open(send_pipe, O_WRONLY);

    // receive msg, modify it, and send it back
    char received_msg[100];
    char new_msg[160];
    read(fd_from_child, received_msg, sizeof(received_msg));
    snprintf(new_msg, sizeof(new_msg), "%sHey, this is process %d with pid: %d\n", received_msg, p_num, getpid());
    write(fd_to_child, new_msg, sizeof(new_msg));

    // close fds
    close(fd_from_child);
    close(fd_to_child);
    return 0;
}