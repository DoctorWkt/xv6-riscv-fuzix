//
// File-system system calls.
// Mostly argument checking, since we don't trust
// user code, and calls into file.c and fs.c.
//

#include <xv6/types.h>
#include <xv6/riscv.h>
#include <xv6/defs.h>
#include <xv6/param.h>
#include <xv6/stat.h>
#include <xv6/spinlock.h>
#include <xv6/proc.h>
#include <xv6/fs.h>
#include <xv6/sleeplock.h>
#include <xv6/file.h>
#include <xv6/fcntl.h>
#include <xv6/errno.h>

// Fetch the nth word-sized system call argument as a file descriptor
// and return both the descriptor and the corresponding struct file.
static int
argfd(int n, int *pfd, struct file **pf)
{
  int fd;
  struct file *f;

  argint(n, &fd);
  if(fd < 0 || fd >= NOFILE || (f=myproc()->ofile[fd]) == 0)
    return -1;
  if(pfd)
    *pfd = fd;
  if(pf)
    *pf = f;
  return 0;
}

// Allocate a file descriptor for the given file.
// Takes over file reference from caller on success.
static int
fdalloc(struct file *f)
{
  int fd;
  struct proc *p = myproc();

  for(fd = 0; fd < NOFILE; fd++){
    if(p->ofile[fd] == 0){
      p->ofile[fd] = f;
      return fd;
    }
  }
  return -1;
}

uint64
sys_dup(void)
{
  struct proc *p = myproc();
  struct file *f;
  int fd;

  if(argfd(0, 0, &f) < 0) {
    p->errno= EBADF;
    return -1;
  }
  if((fd=fdalloc(f)) < 0) {
    p->errno= EMFILE;
    return -1;
  }
  filedup(f);
  return fd;
}

uint64
sys_read(void)
{
  struct proc *proc = myproc();
  struct file *f;
  int n;
  uint64 p;

  argaddr(1, &p);
  argint(2, &n);
  if(argfd(0, 0, &f) < 0) {
    proc->errno= EBADF;
    return -1;
  }
  return fileread(f, p, n);
}

uint64
sys_write(void)
{
  struct proc *proc = myproc();
  struct file *f;
  int n;
  uint64 p;
  
  argaddr(1, &p);
  argint(2, &n);
  if(argfd(0, 0, &f) < 0) {
    proc->errno= EBADF;
    return -1;
  }
  return filewrite(f, p, n);
}

uint64
sys_close(void)
{
  struct proc *p = myproc();
  int fd;
  struct file *f;

  if(argfd(0, &fd, &f) < 0) {
    p->errno= EBADF;
    return -1;
  }
  myproc()->ofile[fd] = 0;
  fileclose(f);
  return 0;
}

uint64
sys_fstat(void)
{
  struct proc *p = myproc();
  struct file *f;
  uint64 st; // user pointer to struct stat

  argaddr(1, &st);
  if(argfd(0, 0, &f) < 0) {
    p->errno= EBADF;
    return -1;
  }
  return filestat(f, st);
}

// Create the path new as a link to the same inode as old.
uint64
sys_link(void)
{
  char name[DIRSIZ], new[MAXPATH], old[MAXPATH];
  struct proc *p = myproc();
  struct inode *dp, *ip;

  if(argstr(0, old, MAXPATH) < 0 || argstr(1, new, MAXPATH) < 0) {
    p->errno = ENOENT;
    return -1;
  }

  begin_op();
  if((ip = namei(old)) == 0){
    end_op();
    p->errno = ENOENT;
    return -1;
  }

  ilock(ip);
  if(ip->type == T_DIR){
    iunlockput(ip);
    end_op();
    p->errno = EPERM;
    return -1;
  }

  ip->nlink++;
  iupdate(ip);
  iunlock(ip);

  if((dp = nameiparent(new, name)) == 0)
    goto bad;
  ilock(dp);
  if(dp->dev != ip->dev || dirlink(dp, name, ip->inum) < 0){
    iunlockput(dp);
    goto bad;
  }
  iunlockput(dp);
  iput(ip);

  end_op();

  return 0;

bad:
  ilock(ip);
  ip->nlink--;
  iupdate(ip);
  iunlockput(ip);
  end_op();
  p->errno = EACCES;
  return -1;
}

// Is the directory dp empty except for "." and ".." ?
static int
isdirempty(struct inode *dp)
{
  int off;
  struct dirent de;

  for(off=2*sizeof(de); off<dp->size; off+=sizeof(de)){
    if(readi(dp, 0, (uint64)&de, off, sizeof(de)) != sizeof(de))
      panic("isdirempty: readi");
    if(de.inum != 0)
      return 0;
  }
  return 1;
}

uint64
sys_unlink(void)
{
  struct proc *p = myproc();
  struct inode *ip, *dp;
  struct dirent de;
  char name[DIRSIZ], path[MAXPATH];
  uint off;

  if(argstr(0, path, MAXPATH) < 0) {
    p->errno = ENOENT;
    return -1;
  }

  begin_op();
  if((dp = nameiparent(path, name)) == 0){
    end_op();
    p->errno = ENOENT;
    return -1;
  }

  ilock(dp);

  // Cannot unlink "." or "..".
  if(namecmp(name, ".") == 0 || namecmp(name, "..") == 0)
    goto bad;

  if((ip = dirlookup(dp, name, &off)) == 0)
    goto bad;
  ilock(ip);

  if(ip->nlink < 1)
    panic("unlink: nlink < 1");
  if(ip->type == T_DIR && !isdirempty(ip)){
    iunlockput(ip);
    goto bad;
  }

  memset(&de, 0, sizeof(de));
  if(writei(dp, 0, (uint64)&de, off, sizeof(de)) != sizeof(de))
    panic("unlink: writei");
  if(ip->type == T_DIR){
    dp->nlink--;
    iupdate(dp);
  }
  iunlockput(dp);

  ip->nlink--;
  iupdate(ip);
  iunlockput(ip);

  end_op();

  return 0;

bad:
  iunlockput(dp);
  end_op();
  p->errno = EPERM;
  return -1;
}

static struct inode*
create(char *path, short type, short major, short minor)
{
  struct inode *ip, *dp;
  char name[DIRSIZ];

  if((dp = nameiparent(path, name)) == 0)
    return 0;

  ilock(dp);

  if((ip = dirlookup(dp, name, 0)) != 0){
    iunlockput(dp);
    ilock(ip);
    if(type == T_FILE && (ip->type == T_FILE || ip->type == T_DEVICE))
      return ip;
    iunlockput(ip);
    return 0;
  }

  if((ip = ialloc(dp->dev, type)) == 0){
    iunlockput(dp);
    return 0;
  }

  ilock(ip);
  ip->major = major;
  ip->minor = minor;
  ip->nlink = 1;
  iupdate(ip);

  if(type == T_DIR){  // Create . and .. entries.
    // No ip->nlink++ for ".": avoid cyclic ref count.
    if(dirlink(ip, ".", ip->inum) < 0 || dirlink(ip, "..", dp->inum) < 0)
      goto fail;
  }

  if(dirlink(dp, name, ip->inum) < 0)
    goto fail;

  if(type == T_DIR){
    // now that success is guaranteed:
    dp->nlink++;  // for ".."
    iupdate(dp);
  }

  iunlockput(dp);

  return ip;

 fail:
  // something went wrong. de-allocate ip.
  ip->nlink = 0;
  iupdate(ip);
  iunlockput(ip);
  iunlockput(dp);
  return 0;
}

uint64
sys_open(void)
{
  char path[MAXPATH];
  struct proc *p = myproc();
  int fd, omode;
  struct file *f;
  struct inode *ip;
  int n;

  argint(1, &omode);
  if((n = argstr(0, path, MAXPATH)) < 0) {
    p->errno = ENOENT;
    return -1;
  }

  begin_op();

  if(omode & O_CREAT){
    ip = create(path, T_FILE, 0, 0);
    if(ip == 0){
      end_op();
      p->errno = EEXIST;
      return -1;
    }
  } else {
    if((ip = namei(path)) == 0){
      p->errno = ENOENT;
      end_op();
      return -1;
    }
    ilock(ip);
    if(ip->type == T_DIR && omode != O_RDONLY){
      iunlockput(ip);
      end_op();
      p->errno = EISDIR;
      return -1;
    }
  }

  if(ip->type == T_DEVICE && (ip->major < 0 || ip->major >= NDEV)){
    iunlockput(ip);
    end_op();
    p->errno = ENODEV;
    return -1;
  }

  if((f = filealloc()) == 0 || (fd = fdalloc(f)) < 0){
    if(f)
      fileclose(f);
    iunlockput(ip);
    end_op();
    p->errno = EINVAL;
    return -1;
  }

  if(ip->type == T_DEVICE){
    f->type = FD_DEVICE;
    f->major = ip->major;
  } else {
    f->type = FD_INODE;
    f->off = 0;
  }
  f->ip = ip;
  f->readable = !(omode & O_WRONLY);
  f->writable = (omode & O_WRONLY) || (omode & O_RDWR);

  if (omode & O_APPEND)
    f->off= f->ip->size;

  if((omode & O_TRUNC) && ip->type == T_FILE){
    itrunc(ip);
  }

  iunlock(ip);
  end_op();

  return fd;
}

uint64
sys_mkdir(void)
{
  char path[MAXPATH];
  struct proc *p = myproc();
  struct inode *ip;

  begin_op();
  if(argstr(0, path, MAXPATH) < 0 || (ip = create(path, T_DIR, 0, 0)) == 0){
    end_op();
    p->errno = EINVAL;
    return -1;
  }
  iunlockput(ip);
  end_op();
  return 0;
}

uint64
sys_mknod(void)
{
  struct inode *ip;
  struct proc *p = myproc();
  char path[MAXPATH];
  int major, minor;

  begin_op();
  argint(1, &major);
  argint(2, &minor);
  if((argstr(0, path, MAXPATH)) < 0 ||
     (ip = create(path, T_DEVICE, major, minor)) == 0){
    end_op();
    p->errno = EINVAL;
    return -1;
  }
  iunlockput(ip);
  end_op();
  return 0;
}

uint64
ichdir(struct inode *ip)
{
  struct proc *p = myproc();
  ilock(ip);
  if(ip->type != T_DIR){
    iunlockput(ip);
    end_op();
    p-> errno = ENOTDIR;
    return -1;
  }
  iunlock(ip);
  iput(p->cwd);
  end_op();
  p->cwd = ip;
  return 0;
}

uint64
sys_chdir(void)
{
  char path[MAXPATH];
  struct inode *ip;
  struct proc *p = myproc();
  
  begin_op();
  if(argstr(0, path, MAXPATH) < 0 || (ip = namei(path)) == 0){
    end_op();
    p-> errno = ENOENT;
    return -1;
  }
  ilock(ip);
  if(ip->type != T_DIR){
    iunlockput(ip);
    end_op();
    p-> errno = ENOTDIR;
    return -1;
  }
  iunlock(ip);
  iput(p->cwd);
  end_op();
  p->cwd = ip;
  return 0;
}

uint64
sys_fchdir(void)
{
  struct file *f;
  struct proc *p = myproc();

  if(argfd(0, 0, &f) < 0) {
    p->errno= EBADF;
    return -1;
  }
  return(ichdir(f->ip));
}

uint64
sys_exec(void)
{
  struct proc *p = myproc();
  char path[MAXPATH], *argv[MAXARG];
  int i;
  uint64 uargv, uarg;

  argaddr(1, &uargv);
  if(argstr(0, path, MAXPATH) < 0) {
    p-> errno = EACCES;
    return -1;
  }
  memset(argv, 0, sizeof(argv));
  for(i=0;; i++){
    if(i >= NELEM(argv)){
      goto bad;
    }
    if(fetchaddr(uargv+sizeof(uint64)*i, (uint64*)&uarg) < 0){
      goto bad;
    }
    if(uarg == 0){
      argv[i] = 0;
      break;
    }
    argv[i] = kalloc();
    if(argv[i] == 0)
      goto bad;
    if(fetchstr(uarg, argv[i], PGSIZE) < 0)
      goto bad;
  }

  int ret = exec(path, argv);

  for(i = 0; i < NELEM(argv) && argv[i] != 0; i++)
    kfree(argv[i]);

  return ret;

 bad:
  for(i = 0; i < NELEM(argv) && argv[i] != 0; i++)
    kfree(argv[i]);
  p->errno = EACCES;
  return -1;
}

uint64
sys_pipe(void)
{
  uint64 fdarray; // user pointer to array of two integers
  struct file *rf, *wf;
  int fd0, fd1;
  struct proc *p = myproc();

  argaddr(0, &fdarray);
  if(pipealloc(&rf, &wf) < 0) {
    p->errno= EMFILE;
    return -1;
  }
  fd0 = -1;
  if((fd0 = fdalloc(rf)) < 0 || (fd1 = fdalloc(wf)) < 0){
    if(fd0 >= 0)
      p->ofile[fd0] = 0;
    fileclose(rf);
    fileclose(wf);
    p->errno= EMFILE;
    return -1;
  }
  if(copyout(p->pagetable, fdarray, (char*)&fd0, sizeof(fd0)) < 0 ||
     copyout(p->pagetable, fdarray+sizeof(fd0), (char *)&fd1, sizeof(fd1)) < 0){
    p->ofile[fd0] = 0;
    p->ofile[fd1] = 0;
    fileclose(rf);
    fileclose(wf);
    p->errno= EMFILE;
    return -1;
  }
  return 0;
}

// lseek code derived from https://github.com/ctdk/xv6
uint64 sys_lseek(void) {
  struct proc *p = myproc();
  int fd;
  int offset;
  int base;
  int newoff= 0;
  int zerosize, i;
  char *zeroed, *z;

  struct file *f;

  if (argfd(0, &fd, &f) < 0) {
    p->errno= EINVAL;
    return(-1);
  }

  argint(1, &offset);
  argint(2, &base);

  if (base == SEEK_SET) {
    newoff = offset;
  }

  if (base == SEEK_CUR)
    newoff = f->off + offset;

  if (base == SEEK_END)
    newoff = f->ip->size + offset;

  if (newoff < 0) {
    p->errno= EINVAL;
    return(-1);
  }

  if (newoff > f->ip->size) {
    zerosize = newoff - f->ip->size;
    zeroed = kalloc();
    z = zeroed;
    for (i = 0; i < PGSIZE; i++)
      *z++ = 0;
    while (zerosize > 0) {
      filewrite(f, (uint64)zeroed, zerosize);
      zerosize -= PGSIZE;
    }
    kfree(zeroed);
  }

  f->off = newoff;
  return(newoff);
}

// For now, only the console
uint64 sys_ioctl(void) {
  struct proc *p = myproc();
  int fd;
  int req;
  uint64 ti;
  struct file *f;

  if (argfd(0, &fd, &f) < 0) {
    p->errno= EINVAL;
    return(-1);
  }

  argint(1, &req);
  argaddr(2, &ti);

  // Test for the console device
  if ((f->type != FD_DEVICE) || (f->major != CONSOLE)) {
    p->errno= ENOTTY;
    return(-1);
  }

  if (consoleioctl()<0) {
    p->errno= EFAULT;
    return(-1);
  }

  return(0);
}
