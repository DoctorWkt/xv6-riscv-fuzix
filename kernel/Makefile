TOP_SRCDIR= ..
include $(TOP_SRCDIR)/Makefile.inc

U= $(TOP_SRCDIR)/cmds/xv6

OBJS = entry.o start.o console.o printf.o uart.o kalloc.o spinlock.o \
  string.o main.o vm.o proc.o swtch.o trampoline.o trap.o syscall.o \
  sysproc.o bio.o fs.o log.o sleeplock.o file.o pipe.o exec.o sysfile.o \
  kernelvec.o plic.o virtio_disk.o

kernel: $(OBJS) kernel.ld $U/initcode
	$(LD) $(LDFLAGS) -T kernel.ld -o kernel $(OBJS) 
	$(OBJDUMP) -S kernel > kernel.asm
	$(OBJDUMP) -t kernel | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > kernel.sym

$U/initcode:
	(cd $U; make initcode)

clean:
	rm -f *.[do]
