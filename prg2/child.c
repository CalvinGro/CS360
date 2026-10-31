#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>  
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char *args[]) {
    
    // read shared file from socket and open it
    int sock = strtol(args[1], NULL, 10);
    char shared_file_name[64];
    read(sock, shared_file_name, sizeof(shared_file_name));
    int shared_file = open(shared_file_name, O_WRONLY | O_APPEND);
    
    // set the shared file to stdout
    int saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout == -1) return 1;
    if (dup2(shared_file, STDOUT_FILENO) == -1) {
        close(shared_file);
        return 1;
    }

    // write child msg to shared file
    printf("-Child- Child %d writing to file.\n", getpid());
    printf("\n-Child- Terminal Info:\n");
    fflush(stdout);

    // print the current terminal's pid, parent-pid, and command to stdout, which is redirected to the file.
    system("ps T -o pid,ppid,comm");

    // create named pipe for child -> p3 (if not already created)
    if (mkfifo("/tmp/fifo_to_p3", 0666) == -1) {
        if (errno != EEXIST) {
            perror("mkfifo fifo_to_p3");
            return 1;
        };
    }

    // create named pipe for child <- p3 (if not already created)
    if (mkfifo("/tmp/fifo_from_p3", 0666) == -1) {
        if (errno != EEXIST) {
            perror("mkfifo fifo_from_p3");
            return 1;
        };
    }

    // create named pipe for child -> p4 (if not already created)
    if (mkfifo("/tmp/fifo_to_p4", 0666) == -1) {
        if (errno != EEXIST) {
            perror("mkfifo fifo_to_p4");
            return 1;
        };
    }

    // create named pipe for child <- p4 (if not already created)
    if (mkfifo("/tmp/fifo_from_p4", 0666) == -1) {
        if (errno != EEXIST) {
            perror("mkfifo fifo_from_p4");
            return 1;
        };
    }

    // open each named pipe for the specific action
    int fd_to_p3 = open("/tmp/fifo_to_p3", O_WRONLY);
    int fd_to_p4 = open("/tmp/fifo_to_p4", O_WRONLY);
    int fd_from_p3 = open("/tmp/fifo_from_p3", O_RDONLY);
    int fd_from_p4 = open("/tmp/fifo_from_p4", O_RDONLY);

    // Send and receive messages from p3 and p4
    char msg[100] = "This is a message from child.\n";
    char msg_p3_modified[100];
    char msg_p4_modified[100];

    write(fd_to_p3, msg, sizeof(msg));
    write(fd_to_p4, msg, sizeof(msg));
    read(fd_from_p3, msg_p3_modified, sizeof(msg_p3_modified));
    read(fd_from_p4, msg_p4_modified, sizeof(msg_p4_modified));

    // close fds
    close(fd_to_p3);
    close(fd_to_p4);
    close(fd_from_p3);
    close(fd_from_p4);

    // unlink named pipes
    unlink("/tmp/fifo_to_p3");
    unlink("/tmp/fifo_to_p4");
    unlink("/tmp/fifo_from_p3");
    unlink("/tmp/fifo_from_p4");

    // write the messages to the shared file
    printf("\n-Child- Original Message:\n%s", msg);
    printf("\n-Child-  Message From P3:\n%s", msg_p3_modified);
    printf("\n-Child-  Message From P4:\n%s", msg_p4_modified);

    // send msg to parent
    char msg_to_parent[50] = "Child has finished.\n";
    write(sock, msg_to_parent, sizeof(msg_to_parent));

    // close socket
    close(sock);

    // close file and restore the stdout fd
    close(shared_file);
    fflush(stdout);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    return 0;
}