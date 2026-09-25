#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(2, "usage: sleep [ticks num]\n");
        exit(1);
    }

    uint n = atoi(argv[1]);
    int ret = sleep(n);
    exit(ret);

}