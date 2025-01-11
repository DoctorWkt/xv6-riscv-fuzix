#include <unistd.h>
#include <stdlib.h>
#include <termios.h>

void cprintf(char *, ...);

int main()
{
  char buf[1000];
  struct termios T, oldT;

  tcgetattr(0, &T);
  tcgetattr(0, &oldT);
  // T.c_lflag &= ~ICANON;	// Turn off canonical mode
  cfmakeraw(&T);		// Turn off canonical mode
  T.c_lflag |= ECHO;		// But leave ECHO on
  tcsetattr(0, TCSANOW, &T);

  while (1) {			// We should get 1 char
    int i=read(0, buf, 1000);	// at a time
    if (i<1) {
      write(2, ".", 1); sleep(1); 
    } else {
      if (buf[0]==4) break;	// Stop on leading ctrl-D
      write(2, buf, i);		// ECHO on, so we see two characters
    }
  }

  tcsetattr(0, TCSANOW, &oldT);	// Restore the original terminal settings
  exit(0);
}
