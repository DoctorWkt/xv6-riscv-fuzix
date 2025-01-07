TOP_SRCDIR= ../..
include $(TOP_SRCDIR)/Makefile.inc

UPROGS= banner basename cal cat cmp cut date dd dirname echo ed \
  factor false fgrep grep head man mkdir more od printenv pwd rm rmdir \
  sleep sum sync tail tee touch tr true uniq uue wc \
  which yes

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
