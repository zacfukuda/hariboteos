OBJS_BOOTPACK = bootpack.o graphic.o dsctbl.o naskfunc.o hankaku.o mysprintf.o int.o

MAKE = make -r
DEL = rm -f

CC = i386-elf-gcc
CFLAGS = -m32 -fno-builtin

default : 
	$(MAKE) img

# Generate files

convHankakuTxt : convHankakuTxt.c
	gcc $< -o $@

hankaku.c : hankaku.txt convHankakuTxt
	./convHankakuTxt

ipl10.bin : ipl10.nas
	nasm $< -o $@ -l ipl10.lst

asmhead.bin : asmhead.nas
	nasm $< -o $@ -l asmhead.lst

naskfunc.o : naskfunc.nas 
	nasm -g -f elf $< -o $@ -l naskfunc.lst

bootpack.hrb : $(OBJS_BOOTPACK) hrb.ld
	$(CC) $(CFLAGS) -march=i486 -nostdlib -T hrb.ld -g $(OBJS_BOOTPACK) -o $@

haribote.sys : asmhead.bin bootpack.hrb Makefile
	cat asmhead.bin bootpack.hrb > haribote.sys

haribote.img : ipl10.bin haribote.sys Makefile
	mformat -f 1440 -C -B ipl10.bin -i haribote.img ::
	mcopy -i haribote.img haribote.sys ::

# Rule

%.o : %.c
	$(CC) $(CFLAGS) -c $*.c -o $*.o

# Commands

img :
	$(MAKE) haribote.img

run :
	$(MAKE) img
	qemu-system-i386 -drive file=haribote.img,format=raw,if=floppy -boot a

clean : 
	-$(DEL) *.bin
	-$(DEL) *.lst
	-$(DEL) *.o
	-$(DEL) *.sys
	-$(DEL) *.hrb
	-$(DEL) hankaku.c
	-$(DEL) convHankakuTxt

src_only :
	$(MAKE) clean
	-$(DEL) haribote.img
