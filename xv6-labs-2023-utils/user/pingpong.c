#include "kernel/types.h"
#include "user/user.h"

int main(void) {
    int pipe1[2];
    int pipe2[2];

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        printf("pipe creation failed\n");
        exit(1);
    }

    int pid = fork();
    if (pid < 0) {
        printf("fork failed\n");
        exit(1);
    }

    if (pid == 0) {
        char buf;
        close(pipe1[1]);
        close(pipe2[0]);

        if (read(pipe1[0], &buf, 1) != 1) {
            printf("child read failed\n");
            exit(1);
        }
        close(pipe1[0]);

        printf("%d: received ping\n", getpid());

        if (write(pipe2[1], &buf, 1) != 1) {
            printf("child write failed\n");
            exit(1);
        }
        close(pipe2[1]);

        exit(0);
    } else { 
        char buf = 'x';
        close(pipe1[0]); 
        close(pipe2[1]); 

        if (write(pipe1[1], &buf, 1) != 1) {
            printf("parent write failed\n");
            exit(1);
        }
        close(pipe1[1]);

        wait(0);

        if (read(pipe2[0], &buf, 1) != 1) {
            printf("parent read failed\n");
            exit(1);
        }
        close(pipe2[0]);

        printf("%d: received pong\n", getpid());

        exit(0);
    }
}

