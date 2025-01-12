TOP_SRCDIR= ../..
include $(TOP_SRCDIR)/Makefile.inc
CFLAGS += -DMINIX

OBJS= alias.o bind.o builtin.o clebuf.o clex.o comlined.o exec.o file.o \
  hist.o job.o main.o malloc.o meta.o parse.o prints.o signal.o \
  term.o tinytcap.o val.o var.o

wish: $(OBJS)
	$(LD) $(LDFLAGS) -T ../user.ld -o $@ $(CRT0) $(OBJS) -L$(LIBDIR) -lc
	$(OBJDUMP) -S $@ > $*.asm
	$(OBJDUMP) -t $@ | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > $*.sym

install: wish
	$(STRIP) wish
	cp wish $(TOP_SRCDIR)/fs/bin

clean:
	rm -f *.[do] s *.asm
