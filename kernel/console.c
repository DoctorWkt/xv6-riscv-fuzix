//
// Console input and output, to the uart.
// Reads are line at a time.
// Implements special input characters:
//   newline -- end of line
//   control-h -- backspace
//   control-u -- kill line
//   control-d -- end of file
//   control-p -- print process list
//

#include <stdarg.h>
#include <xv6/types.h>
#include <xv6/param.h>
#include <xv6/spinlock.h>
#include <xv6/sleeplock.h>
#include <xv6/fs.h>
#include <xv6/file.h>
#include <xv6/memlayout.h>
#include <xv6/riscv.h>
#include <xv6/defs.h>
#include <xv6/proc.h>
#include <xv6/termios.h>
#include <xv6/errno.h>

#define BACKSPACE 0x100
#define C(x)  ((x)-'@')  // Control-x

//
// send one character to the uart.
// called by printf(), and to echo input characters,
// but not from write().
//
void
consputc(int c)
{
  if(c == BACKSPACE){
    // if the user typed backspace, overwrite with a space.
    uartputc_sync('\b'); uartputc_sync(' '); uartputc_sync('\b');
  } else {
    uartputc_sync(c);
  }
}

struct {
  struct spinlock lock;
  
  // input
#define INPUT_BUF_SIZE 128
  char buf[INPUT_BUF_SIZE];
  uint r;  // Read index
  uint w;  // Write index
  uint e;  // Edit index
  struct termios termios;
} cons;

void
consechoc(int c)
{
  if(c != C('D') && cons.termios.c_lflag & ECHO)
    consputc(c);
}

//
// user write()s to the console go here.
//
int
consolewrite(int user_src, uint64 src, int n)
{
  int i;

  for(i = 0; i < n; i++){
    char c;
    if(either_copyin(&c, user_src, src+i, 1) == -1)
      break;
    uartputc(c);
  }

  return i;
}

//
// user read()s from the console go here.
// copy (up to) a whole input line to dst.
// user_dist indicates whether dst is a user
// or kernel address.
//
int
consoleread(int user_dst, uint64 dst, int n)
{
  uint target;
  int c;
  char cbuf;

  target = n;
  acquire(&cons.lock);
  while(n > 0){
    // wait until interrupt handler has put some
    // input into cons.buffer.
    while(cons.r == cons.w){
      if(killed(myproc())){
        release(&cons.lock);
        return -1;
      }
      sleep(&cons.r, &cons.lock);
    }

    c = cons.buf[cons.r++ % INPUT_BUF_SIZE];

    if(c == C('D') && cons.termios.c_lflag & ICANON){  // end-of-file
      if(n < target){
        // Save ^D for next time, to make sure
        // caller gets a 0-byte result.
        cons.r--;
      }
      break;
    }

    // copy the input byte to the user-space buffer.
    cbuf = c;
    if(either_copyout(user_dst, dst, &cbuf, 1) == -1)
      break;

    dst++;
    --n;

    if(c == '\n' || ((cons.termios.c_lflag & ICANON)==0)) {
      // A whole line has arrived, return to the user-level read().
      // Or, we are not in canonical mode, return individual characters.
      break;
    }
  }
  release(&cons.lock);

  return target - n;
}

//
// the console input interrupt handler.
// uartintr() calls this for input character.
// do erase/kill processing, append to cons.buf,
// wake up consoleread() if a whole line has arrived.
//
void
consoleintr(int c)
{
  acquire(&cons.lock);

 if(cons.termios.c_lflag & ICANON){
  switch(c){
  case C('P'):  // Print process list.
    procdump();
    break;
  case C('U'):  // Kill line.
    while(cons.e != cons.w &&
          cons.buf[(cons.e-1) % INPUT_BUF_SIZE] != '\n'){
      cons.e--;
      consechoc(BACKSPACE);
    }
    break;
  case C('H'): // Backspace
  case '\x7f': // Delete key
    if(cons.e != cons.w){
      cons.e--;
      consechoc(BACKSPACE);
    }
    break;
  default:
    if(c != 0 && cons.e-cons.r < INPUT_BUF_SIZE){
      c = (c == '\r') ? '\n' : c;

      // echo back to the user.
      consechoc(c);

      // store for consumption by consoleread().
      cons.buf[cons.e++ % INPUT_BUF_SIZE] = c;

      if(c == '\n' || c == C('D') || cons.e-cons.r == INPUT_BUF_SIZE){
        // wake up consoleread() if a whole line (or end-of-file)
        // has arrived.
        cons.w = cons.e;
        wakeup(&cons.r);
      }
    }
    break;
  }
 } else {	// Not canonical input
    if(c != 0 && cons.e-cons.r < INPUT_BUF_SIZE){
      // echo back to the user.
      consechoc(c);

      // store for consumption by consoleread().
      cons.buf[cons.e++ % INPUT_BUF_SIZE] = c;

      // wake up consoleread()
      // has arrived.
      cons.w = cons.e;
      wakeup(&cons.r);
    }
 }
  release(&cons.lock);
}

uint64 consoleioctl(void)
{
  int req;
  uint64 ti; 			// user pointer to struct termios
  struct proc *p = myproc();

  // Get the arguments
  argint(1, &req);
  argaddr(2, &ti);

  // Is the request permitted?
  if (req != TIOCGETA && req != TIOCSETA) {
    return -1;
  }

  if (req == TIOCGETA) {
    if (copyout(p->pagetable, ti,
		(char *) &(cons.termios), sizeof(struct termios))<0)
      return(-1);
  } else {
    if (copyin(p->pagetable, (char *) &(cons.termios), 
		ti, sizeof(struct termios))<0) {
      return(-1);
    }
  }
  return(0);
}

// This should go in another file

// Read from /dev/zero
int
zeroread(int user_dst, uint64 dst, int n)
{
  int i;
  char ch= 0;
  for (i=0; i<n; i++, dst++) {
    // copy the input byte to the user-space buffer.
    if (either_copyout(user_dst, dst, &ch, 1) == -1)
      break;
  }
  return(i);
}

// Write to /dev/null
static int
nullwrite(int user_src, uint64 src, int n)
{
  return(n);
}

void
consoleinit(void)
{
  initlock(&cons.lock, "cons");

  uartinit();

  // connect read and write system calls
  // to consoleread and consolewrite.
  devsw[CONSOLE].read  = consoleread;
  devsw[CONSOLE].write = consolewrite;
  devsw[DEVNULL].read  = zeroread;
  devsw[DEVNULL].write = nullwrite;
  cons.termios.c_lflag = ECHO | ICANON;
}
