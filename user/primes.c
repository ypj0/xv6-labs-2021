#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int p) {
    int buf;
    if(read(p, &buf, sizeof(buf)) == 0) {
        close(p);
        exit(0);
    }

    fprintf(1, "prime %d\n", buf);

    int pp[2];
    pipe(pp);

    if(fork() == 0) {
        close(pp[1]);
        close(p);
        primes(pp[0]);
    } else {
        close(pp[0]);
        int num;
        while(read(p, &num, sizeof(num)) > 0) {
            if(num % buf != 0) {
                write(pp[1], &num, sizeof(num));
            }
        }
        close(p);
        close(pp[1]);
        wait(0);
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    if(argc != 1) {
        fprintf(1, "usage: [primes]\n");
        exit(1);
    }

    int p[2];
    pipe(p);

    if(fork() == 0) {
        close(p[1]);
        primes(p[0]);
    } else {
        close(p[0]);
        for(int i = 2; i <= 35; ++i) {
            write(p[1], &i, sizeof(i));
        }
        close(p[1]);
        wait(0);
    }
    exit(0);
}