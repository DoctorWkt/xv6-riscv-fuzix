TOP_SRCDIR= ..
include $(TOP_SRCDIR)/Makefile.inc

all:
	(cd ulib; make)
	mv ulib/crt0.o .
	$(AR) vr libulib.a ulib/*.o
	$(RANLIB) libulib.a

clean:
	(cd ulib; make clean)
	rm -f *.a *.o
