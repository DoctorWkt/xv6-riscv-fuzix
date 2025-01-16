#define T_DIR     1   // Directory
#define T_FILE    2   // File
#define T_DEVICE  3   // Device
#define T_PIPE    4   // Pipe

struct stat {
  int dev;     // File system's disk device
  uint ino;    // Inode number
  short type;  // Type of file
  short nlink; // Number of links to file
  short major; // Device's major and minor numbers
  short minor;
  int mtime;   // Last modification time
  uint64 size; // Size of file in bytes
};
