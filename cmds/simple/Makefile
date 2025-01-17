TOP_SRCDIR= ../..
include $(TOP_SRCDIR)/Makefile.inc
CFLAGS += -Wno-infinite-recursion -Wno-char-subscripts

UPROGS= banner basename cal cat cmp cp cut date dd dirname echo ed \
  factor false fgrep grep head less ls man mkdir more od printenv pwd rm rmdir \
  oldsh sed sleep sort sum sync tail tar tee touch tr true uniq uud uue wc \
  which xargs yes roff

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

sh: sh.o
	$(LD) $(LDFLAGS) -T ../user.ld -o $@ $(CRT0) $< -L$(LIBDIR) -lc -lreadline
	$(OBJDUMP) -S $@ > $*.asm
	$(OBJDUMP) -t $@ | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > $*.sym

clean:
	rm -f *.[do] *.asm *.sym $(UPROGS)
