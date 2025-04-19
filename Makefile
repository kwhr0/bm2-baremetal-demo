#CC = chibicc
CC = cc68
MPU = 6800

ifeq ($(CC),chibicc)
AS = as6800
OBJS = crt0-c.o memcpy.o memset.o
CFLAGS = -I/opt/cc68/include
LD = ld6800
LIBC = /opt/chibicc/lib/libc.a
else
AS = as68
OBJS = crt0.o
CFLAGS = -m$(MPU) -I.
LD = ld68
LIBC = /opt/cc68/lib/lib$(MPU).a
endif

OBJS += main.o base.o graph.o util.o
START = 4000

.SUFFIXES: .asm

all: a.bin

a.bin: $(OBJS)
	$(LD) -b -C 0x$(START) -m a.map $^ $(LIBC)
	dd if=a.out of=a.bin bs=0x$(START) skip=1 >/dev/null 2>&1
	f9dasm -$(MPU) -offset $(START) a.bin > a.lst

.asm.o:
	$(AS) -l $(<:.asm=.lst) $<

.c.o:
	$(CC) $(CFLAGS) -S $<
	$(AS) -l $(<:.c=.lst) $(<:.c=.s)

clean:
	rm -f a.{bin,map,out} *.{lst,s,o}

main.o: types.h base.h graph.h
base.o: types.h base.h
graph.o: types.h graph.h
util.o: types.h util.h
