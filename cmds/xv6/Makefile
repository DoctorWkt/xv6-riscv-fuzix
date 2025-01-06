TOP_SRCDIR= ../..
include $(TOP_SRCDIR)/Makefile.inc

UPROGS=\
	_cat\
	_echo\
	_forktest\
	_grep\
	_init\
	_kill\
	_ln\
	_ls\
	_mkdir\
	_rm\
	_sh\
	_stressfs\
	_usertests\
	_grind\
	_wc\
	_zombie\

all: initcode $(UPROGS)

install: all
	$(STRIP) $(UPROGS)
	cp $(UPROGS) $(TOP_SRCDIR)/fs/bin

initcode: initcode.S
	$(CC) $(CFLAGS) -march=rv64g -nostdinc -I. -Ikernel -c \
		initcode.S -o initcode.o
	$(LD) $(LDFLAGS) -N -e start -Ttext 0 -o initcode.out initcode.o
	$(OBJCOPY) -S -O binary initcode.out initcode
	$(OBJDUMP) -S initcode.o > initcode.asm

# Cancel the .c -> output implicit rule
%: %.c

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

_%: %.o
	$(LD) $(LDFLAGS) -T ../user.ld -o $@ $(CRT0) $< -L$(LIBDIR) -lulib
	$(OBJDUMP) -S $@ > $*.asm
	$(OBJDUMP) -t $@ | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > $*.sym

usys.S : usys.pl
	perl usys.pl > usys.S

usys.o : usys.S
	$(CC) $(CFLAGS) -c -o usys.o usys.S

_forktest: forktest.o
	# forktest has less library code linked in - needs to be small
	# in order to be able to max out the proc table.
	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _forktest forktest.o \
		$(TOP_SRCDIR)/lib/ulib/ulib.o \
		$(TOP_SRCDIR)/lib/ulib/usys.o
	$(OBJDUMP) -S _forktest > forktest.asm

# Prevent deletion of intermediate files, e.g. cat.o, after first build, so
# that disk image changes after first build are persistent until clean.  More
# details:
# http://www.gnu.org/software/make/manual/html_node/Chained-Rules.html
.PRECIOUS: %.o

clean:
	rm -f *.[do] *.asm *.sym initcode initcode.out \
        usys.S $(UPROGS)
