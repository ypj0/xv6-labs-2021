#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

void run_command(char* command, char** args, int argcount, char* line) {
    char* line_args[MAXARG];
    int line_argc = 0;

    char *p = line;
    while(*p) {
        while(*p == ' ' || *p == '\t')p++; //遍历行，遇到空格或制表符就当作分隔符
        if(*p == 0) break;
        line_args[line_argc++] = p;
        while(*p && *p != ' ') p++;
        if(*p) {
            *p = 0;
            p++;
        }
    }

    if(line_argc == 0) return;

    char *argv[MAXARG];
    int idx = 0;
    argv[idx++] = command;
    for(int i = 0; i < argcount; ++i) {
        if(idx >= MAXARG - 1) break;
        argv[idx++] = args[i];
    }
    for(int i = 0; i < line_argc; ++i) {    //拼接标准输入的参数
        if(idx >= MAXARG - 1) break;
        argv[idx++] = line_args[i];
    }
    argv[idx] = 0;

    if(fork() == 0) {   //fork + exec
        exec(command, argv);
        fprintf(2, "xargs: exec %s failed\n", command);
        exit(1);
    } else {
        wait(0);
    }
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        fprintf(2, "usage: xargs command [args...]\n");
        exit(1);
    }

    char* command = argv[1];
    char *args[MAXARG];
    int argcount = argc - 2;
    for(int i = 0; i < argcount; ++i) {
        args[i] = argv[i + 2];
    }

    char buf[512];
    char c;
    int n = 0;
    while(read(0, &c, 1) == 1) {  //按行处理
        if(c == '\n') {
            buf[n] = 0;
            run_command(command, args, argcount, buf);
            n = 0;
        } else {
            if(n < sizeof(buf) - 1) {
                buf[n++] = c;
            }
        }
    }

    if(n > 0) { //恰好只有一行
        buf[n] = 0;
        run_command(command, args, argcount, buf);
    }
    exit(0);
}
