TOP_SRCDIR= ../..
include $(TOP_SRCDIR)/Makefile.inc

OBJS = access.o chmod.o chown.o creat.o dup2.o exec.o fstat.o \
  getenv.o gethostname.o getpagesize.o getpgid.o \
  gettimeofday.o getuid.o misc.o mkfifo.o opendir.o rename.o rmdir.o \
  signal.o stat.o umask.o utime.o vfork.o waitpid.o brk.o cprintf.o \
  locale.o usys.o crt0.o

all: $(OBJS)

install: $(OBJS)
	$(AR) vr ../libc.a $(OBJS)
	$(RANLIB) ../libc.a

usys.S: usys.pl
	perl usys.pl > usys.S

usys.o: usys.S
	$(CC) $(CFLAGS) -c -o usys.o usys.S

clean:
	rm -f *.[do] usys.S
