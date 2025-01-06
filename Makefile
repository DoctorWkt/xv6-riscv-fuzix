TOP_SRCDIR= .
include Makefile.inc

U=cmds/xv6
K=kernel

all:
	(cd kernel; make)

mkfs/mkfs:
	(cd mkfs; make)

fs.img: mkfs/mkfs README
	(cd kernel; make)
	(cd lib; make)
	(cd cmds; make install)
	mkfs/mkfs fs.img fs

clean:
	(cd kernel; make clean)
	(cd lib; make clean)
	(cd cmds; make clean)
	(cd mkfs; make clean)
	rm -f fs.img
	rm -f fs/bin/*

# try to generate a unique GDB port
GDBPORT = $(shell expr `id -u` % 5000 + 25000)
# QEMU's gdb stub command line changed in 0.11
QEMUGDB = $(shell if $(QEMU) -help | grep -q '^-gdb'; \
	then echo "-gdb tcp::$(GDBPORT)"; \
	else echo "-s -p $(GDBPORT)"; fi)
ifndef CPUS
CPUS := 3
endif

QEMUOPTS = -machine virt -bios none -kernel $K/kernel -m 128M -smp $(CPUS) -nographic
QEMUOPTS += -global virtio-mmio.force-legacy=false
QEMUOPTS += -drive file=fs.img,if=none,format=raw,id=x0
QEMUOPTS += -device virtio-blk-device,drive=x0,bus=virtio-mmio-bus.0

kernel/kernel:
	(cd kernel; make)

qemu: fs.img kernel/kernel
	$(QEMU) $(QEMUOPTS)

.gdbinit: .gdbinit.tmpl-riscv
	sed "s/:1234/:$(GDBPORT)/" < $^ > $@

qemu-gdb: .gdbinit fs.img
	@echo "*** Now run 'gdb' in another window." 1>&2
	$(QEMU) $(QEMUOPTS) -S $(QEMUGDB)
