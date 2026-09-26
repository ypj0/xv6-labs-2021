#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if(argc != 1) {
        fprintf(2, "usage: [pingpong]\n");
        exit(1);
    }

    int p2c[2]; // parent->child
    int c2p[2]; // child -> parent
    pipe(p2c);
    pipe(c2p);

    if(fork() == 0) {
        close(p2c[1]);
        close(c2p[0]);

        char buf[1];
        int n = read(p2c[0], buf, sizeof(buf));

        if(n > 0) {
            fprintf(1, "%d: received ping\n", getpid());
            close(p2c[0]);
            write(c2p[1], buf, sizeof(buf));
            close(c2p[1]);
            exit(0);
        } 
        exit(1);
    } else {
        close(p2c[0]);
        close(c2p[1]);

        char buf[1] = {'a'};
        write(p2c[1], buf, sizeof(buf));
        close(p2c[1]);
        int n = read(c2p[0], buf, sizeof(buf));
        if(n > 0) {
            fprintf(1, "%d: received pong\n", getpid());
        }
        close(c2p[0]);
        wait(0);
        exit(n > 0 ? 0: 1);
    }
}