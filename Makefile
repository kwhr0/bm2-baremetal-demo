MPU = 6800
START = 4000

.SUFFIXES: .asm

all: a.bin

a.bin: crt0.o main.o base.o graph.o util.o
	ld68 -b -C 0x$(START) $^ /opt/cc68/lib/lib$(MPU).a
	dd if=a.out of=a.bin bs=0x$(START) skip=1
	f9dasm -$(MPU) -offset $(START) a.bin > a.lst

.asm.o:
	as68 -l $(<:.asm=.lst) $<

.c.o:
	cc68 -m$(MPU) -I. -S $<
	as68 -l $(<:.c=.lst) $(<:.c=.s)

clean:
	rm -f a.{bin,out} *.{lst,s,o}

main.o: types.h base.h graph.h
base.o: types.h base.h
graph.o: types.h graph.h
util.o: types.h util.h
