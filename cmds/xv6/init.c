// init: The initial user-level program

#include <xv6/types.h>
#include <xv6/stat.h>
#include <xv6/spinlock.h>
#include <xv6/sleeplock.h>
#include <xv6/fs.h>
#include <xv6/file.h>
#include <xv6/user.h>
#include <xv6/fcntl.h>

char *argv[] = { "/bin/sh", 0 };

int
main(void)
{
  int pid, wpid;

  // Make and populate a /dev
  mkdir("/dev");
  mknod("/dev/tty", CONSOLE, 0);
  mknod("/dev/null", DEVNULL, 0);
  mknod("/dev/zero", DEVNULL, 0);

  open("/dev/tty", O_RDWR);	// stdin:  fd 0
  dup(0);			// stdout: fd 1
  dup(0);			// stderr: fd 2

  for(;;){
    printf("init: starting sh\n");
    pid = fork();
    if(pid < 0){
      printf("init: fork failed\n");
      _exit(1);
    }
    if(pid == 0){
      exec("/bin/sh", argv);
      printf("init: exec sh failed\n");
      _exit(1);
    }

    for(;;){
      // this call to wait() returns if the shell _exits,
      // or if a parentless process _exits.
      wpid = wait((int *) 0);
      if(wpid == pid){
        // the shell _exited; restart it.
        break;
      } else if(wpid < 0){
        printf("init: wait returned an error\n");
        _exit(1);
      } else {
        // it was a parentless process; do nothing.
      }
    }
  }
}
