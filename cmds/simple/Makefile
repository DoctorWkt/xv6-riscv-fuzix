TOP_SRCDIR= ../..
include $(TOP_SRCDIR)/Makefile.inc
CFLAGS += -Wno-error=char-subscripts

UPROGS= banner basename cal cat cmp cp cut date dd dirname echo ed \
  factor false fgrep grep head ls man mkdir more od printenv pwd rm rmdir \
  sed sleep sort sum sync tail tar tee touch tr true uniq uud uue vile wc \
  which xargs yes

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

vile: vile.o
	$(LD) $(LDFLAGS) -T ../user.ld -o $@ $(CRT0) $< -L$(LIBDIR) \
		-ltermcap -lcurses -lc
	$(OBJDUMP) -S $@ > $*.asm
	$(OBJDUMP) -t $@ | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > $*.sym

clean:
	rm -f *.[do] *.asm *.sym $(UPROGS)
