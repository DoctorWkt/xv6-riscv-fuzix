SHELL = /bin/sh

TOP_SRCDIR = ../..
include $(TOP_SRCDIR)/Makefile.inc
CFLAGS += -Wno-char-subscripts
#CFLAGS=-g

OBJS=args.o blok.o builtin.o cmd.o ctype.o error.o expand.o \
	fault.o io.o macro.o msg.o name.o print.o service.o setbrk.o \
	stak.o string.o word.o xec.o xv6env.o
HDRS=ctype.h defs.h mac.h mode.h name.h stak.h sym.h timeout.h \
  shfeatures.h

### define next to use included here alloc/malloc/realloc
ALLOCLIB = sbrkalloc.o
#ALLOCLIB = mmapalloc.o

AFLAGS  =
CFLAGS += $(AFLAGS)
SHELL_NAME = /bin/bsh

all:	v7sh

$(OBJS): $(HDRS)
service.o: sort.h signames.h
sbrkalloc.o: shfeatures.h mmapalloc.h
setbrk.o: db_setbrk.h
blok.o: db_blok.h

v7sh: main.o $(OBJS) sbrkalloc.o
	$(LD) $(LDFLAGS) -T ../user.ld -o $@ $(CRT0) main.o $(OBJS) sbrkalloc.o -L$(LIBDIR) -lc
	$(OBJDUMP) -S $@ > $*.asm
	$(OBJDUMP) -t $@ | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > $*.sym

install: v7sh
	$(STRIP) v7sh
	cp v7sh ../../fs/bin/sh

db_%.h:
	@ln -s Memory/$@ $@
%alloc.c:
	@ln -s Memory/$@ $@
mmapalloc.h:
	@ln -s Memory/$@ $@
tryreaddir.c:
	@ln -s Memory/$@ $@
mkfiles.c:
	@ln -s Memory/$@ $@
signames.c:
	@ln -s Memory/$@ $@
sort.h:
	@ln -s Memory/$@ $@


mlib:
	echo 'sbrkalloc.o' > mlib

tests: mkfiles sh
	@rm -f Z_[a-q]*;
	./mkfiles
	env -i ./sh -c 'PATH=/bin:/usr/bin; echo "cat << EOF" > XX'
	env -i ./sh -c 'PATH=/bin:/usr/bin; echo Z_* >> XX'
	env -i ./sh -c 'PATH=/bin:/usr/bin; echo EOF >> XX'
	env -i ./sh XX
	env -i ./sh -c 'PATH=/bin:/usr/bin; echo *; date; echo *.h *.c; ps'
	@rm -f Z_[a-q]*

clean:
	rm -rf *.[do] *.a sh v7sh

MYARCH:=$(shell uname -m | sed -e 's/i[4-9]86/i386/' -e 's/armv[3-6]t\?e\?[lb]/arm/')
dietlibc/bin-$(MYARCH)/diet:
	cvs -d :pserver:cvs@cvs.fefe.de:/cvs -z9 co dietlibc
	cd dietlibc && make
dietbuild: dietlibc/bin-$(MYARCH)/diet
	DIETHOME=$(CURDIR)/dietlibc make CC="./$< -Os gcc -nostdinc"

ME = $(notdir $(CURDIR))
tgz:
	date -u > Version
	cd .. && tar -cvzf $(ME).tar.gz --owner=root --group=root \
 --exclude=$(ME)/dietlibc $(ME)/*
