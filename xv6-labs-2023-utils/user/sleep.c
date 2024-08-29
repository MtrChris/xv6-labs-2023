#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if(argc != 2) {
        fprintf(2, "Usage: sleep [number]\n");
        exit(1);
    }

    int ticks = atoi(argv[1]);
    if(ticks < 0) {
        fprintf(2, "sleep: number of ticks must be non-negative\n");
        exit(1);
    }

    sleep(ticks);
    exit(0);
}

