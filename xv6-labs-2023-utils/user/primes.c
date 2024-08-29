#include "kernel/types.h"
#include "user/user.h"

void filter(int);

int main(void) {
    int numbers[2]; // Pipe to generate numbers
    pipe(numbers);

    if (fork() == 0) {
        // Child: close write end of numbers, move to filter stage
        close(numbers[1]);
        filter(numbers[0]);
    } else {
        // Parent: generate numbers and write to the pipe
        close(numbers[0]); // Close read-end since parent is writing
        for (int i = 2; i <= 35; i++) {
            write(numbers[1], &i, sizeof(int));
        }
        close(numbers[1]); // Done generating numbers
        wait(0); // Wait for the pipeline to finish
        exit(0);
    }
    return 0;
}

void filter(int read_side) {
    int prime;
    // Read the first number, which is a prime for this stage
    if (read(read_side, &prime, sizeof(int)) != sizeof(int)) {
        close(read_side);
        exit(0);
    }

    printf("prime %d\n", prime);

    int next_pipe[2];
    pipe(next_pipe);

    if (fork() == 0) {
        // Child: filter further
        close(next_pipe[1]); // Close write end in child
        filter(next_pipe[0]); // Recursively filter
    } else {
        // Parent (current filter): read and filter numbers
        close(next_pipe[0]); // Close read end in parent
        
        int num;
        while (read(read_side, &num, sizeof(int)) == sizeof(int)) {
            if (num % prime != 0) {
                write(next_pipe[1], &num, sizeof(int));
            }
        }
        close(read_side);
        close(next_pipe[1]);
        wait(0); // Wait for child processes
        exit(0);
    }
}

