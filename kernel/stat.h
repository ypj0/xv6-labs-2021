#define T_DIR     1   // Directory
#define T_FILE    2   // File
#define T_DEVICE  3   // Device

//描述inode对应的文件是什么，有多大，在哪里
struct stat {
  int dev;     // File system's disk device， 文件位置
  uint ino;    // Inode number， inode
  short type;  // Type of file， 标识文件的类型
  short nlink; // Number of links to file， 链接计数，有多少给inum指向这个inode
  uint64 size; // Size of file in bytes， 文件的实际字节数
};
