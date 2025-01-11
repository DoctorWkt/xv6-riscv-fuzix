#include <termios.h>
#include <unistd.h>


int tcsetattr(int fd, int oa, const struct termios *termios_p)
{
  return ioctl(fd, TIOCSETA, termios_p);
}

