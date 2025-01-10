TOP_SRCDIR= ../..
include $(TOP_SRCDIR)/Makefile.inc
CFLAGS += -Wno-infinite-recursion

UPROGS= seektest\
	tryexit\
	errtest\
	tryraw\
	isattytest\
	prtime\
	test022\
	cptest\
	crtrunc\
	stdiotest\
	prftest\
	perrtest\
	brktest\
	malltest\
	recurtest\

all: $(UPROGS)

install: all
	$(STRIP) $(UPROGS)
	cp $(UPROGS) $(TOP_SRCDIR)/fs/bin

# Cancel the .c -> output implicit rule
%: %.c

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

%: %.o
	$(LD) $(LDFLAGS) -T ../user.ld -o $@ $(CRT0) $< -L$(LIBDIR) -lc
	$(OBJDUMP) -S $@ > $*.asm
	$(OBJDUMP) -t $@ | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > $*.sym

clean:
	rm -f *.[do] *.asm *.sym $(UPROGS)
