#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* extract(char* file) { //提取文件名
    char* p = file + strlen(file); //'\0'
    while(p > file && *p != '/') {
        p--;
    }
    if(*p == '/') return p + 1;
    return file;
}

void find(char* path, char* file) {
    int fd = open(path, 0);
    struct dirent de;
    struct stat st;
    char buf[512], *p;
    if(fd < 0) {
        fprintf(2, "cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0) {
        fprintf(2, "cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type) {
    case T_FILE:    //如果恰好是需要的文件，直接打印
        if(strcmp(extract(path), file) == 0) {
            printf("%s\n", path);
        }
        close(fd);
        break;

    case T_DIR:
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0) continue; //被删除的文件，跳过

            char name[DIRSIZ + 1];
            memmove(name, de.name, DIRSIZ);
            name[DIRSIZ] = 0;
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) //跳过.和..
                continue;

            memmove(p, name, DIRSIZ);
            p[DIRSIZ] = 0;
            find(buf, file); //进入下级目录继续寻找
        }
    break;
    }
    close(fd);
}

int
main(int argc, char *argv[])
{
    if(argc != 3) {
        fprintf(2, "usage: [Dont find path file]\n");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}