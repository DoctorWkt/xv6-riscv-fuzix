TOP_SRCDIR= ../..
include $(TOP_SRCDIR)/Makefile.inc

ULIB = crt0.o usys.o printf.o umalloc.o ulib.o umalloc.o

all: $(ULIB)

usys.S: usys.pl
	perl usys.pl > usys.S

usys.o: usys.S
	$(CC) $(CFLAGS) -c -o usys.o usys.S

clean:
	rm -f *.[do] usys.S
