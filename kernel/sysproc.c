#include <xv6/types.h>
#include <xv6/riscv.h>
#include <xv6/defs.h>
#include <xv6/param.h>
#include <xv6/memlayout.h>
#include <xv6/spinlock.h>
#include <xv6/proc.h>
#include <xv6/errno.h>

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  struct proc *p = myproc();
  int n;

  argint(0, &n);
  addr = p->sz;
  if(growproc(n) < 0) {
    p->errno= ENOMEM;
    return -1;
  }
  return addr;
}

uint64
sys_sleep(void)
{
  struct proc *p = myproc();
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;

  // Ticks are about 10Hz, so we multiply n by 10
  n= n * 10;

  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      p->errno= EINVAL;
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
