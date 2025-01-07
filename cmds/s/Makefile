TOP_SRCDIR= ../..
include $(TOP_SRCDIR)/Makefile.inc

OBJS= s.o address.o adjust.o Bman.o buffer.o commands.o \
      keyboard.o lib.o operator.o screen.o Sman.o yank.o

s: $(OBJS)
	$(LD) $(LDFLAGS) -T ../user.ld -o $@ $(CRT0) $(OBJS) -L$(LIBDIR) -lc
	$(OBJDUMP) -S $@ > $*.asm
	$(OBJDUMP) -t $@ | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > $*.sym

install: s
	cp s $(TOP_SRCDIR)/fs/bin/vi

clean:
	rm -f *.[do] s *.asm
