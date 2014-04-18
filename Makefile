AS=as -Iinclude
LD=ld
CC=gcc
CPP=gcc -E -nostdinc -I include
CFLAGS=-Wall -pedantic -W -nostdlib -nostdinc -Wno-long-long -I include -fomit-frame-pointer

KERNEL_OBJS= load.o main.o ports.o video.o

#.c.s:
#	${CC} ${CFLAGS} -S -o $*.s $<

.s.o:
	${AS} -a $< -o $*.o >$*.map

all: final.img 

final.img: bootsect kernel
	cat bootsect kernel > final.img
	@wc -c final.img

bootsect: bootsect.o
	${LD} --oformat binary -N -e start -Ttext 0x7c00 -o bootsect $<

kernel: ${KERNEL_OBJS}
	${LD} --oformat binary -N -e pm_mode -Ttext 0x0000 -o $@ ${KERNEL_OBJS}
	@wc -c kernel

clean:
	rm -f *.img kernel bootsect *.o *.map

dep:
	sed '/\#\#\# Dependencies/q' < Makefile > tmp_make
	(for i in *.c;do ${CPP} -M $$i;done) >> tmp_make
	mv tmp_make Makefile

### Dependencies:
main.o: main.c
ports.o: ports.c
video.o: video.c
