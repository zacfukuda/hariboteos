LIBPATH = ../lib/

OBJS_BOOTPACK = bootpack.o naskfunc.o hankaku.o graphic.o dsctbl.o int.o fifo.o keyboard.o mouse.o memory.o sheet.o timer.o mtask.o window.o console.o file.o $(LIBPATH)libstdio.a $(LIBPATH)libstring.a

MAKE = make -r
DEL = rm -f

CC = i386-elf-gcc
CFLAGS = -m32 -fno-builtin
COPTION = -march=i486 -nostdlib

default :
	$(MAKE) ipl10.bin
	$(MAKE) haribote.sys

ipl10.bin : ipl10.nas
	nasm $< -o $@ -l ipl10.lst

asmhead.bin : asmhead.nas
	nasm $< -o $@ -l asmhead.lst

convHankakuTxt : convHankakuTxt.c
	gcc $< -o $@

hankaku.c : hankaku.txt convHankakuTxt
	./convHankakuTxt

# https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html
bootpack.hrb : $(OBJS_BOOTPACK) hrb.ld
	$(CC) $(CFLAGS) $(COPTION) -T hrb.ld -Xlinker -Map=bootpack.map -g $(OBJS_BOOTPACK) -o $@

haribote.sys : asmhead.bin bootpack.hrb
	cat asmhead.bin bootpack.hrb > haribote.sys

# Rule

# C -> Object
%.o : %.c
	$(CC) $(CFLAGS) -c $*.c -o $*.o

# NASM -> Object
# -g:generate debug, -f:format, -o:outfile, -l:listfile, elf:ELF32 (i386)
%.o : %.nas
	nasm -g -f elf $*.nas -o $*.o

# Remove
clean :
	-$(DEL) asmhead.bin
	-$(DEL) convHankakuTxt
	-$(DEL) hankaku.c
	-$(DEL) *.lst
	-$(DEL) *.o
	-$(DEL) *.sys
	-$(DEL) *.hrb
	-$(DEL) *.map
	
src_only :
	$(MAKE) clean
	-$(DEL) ipl10.bin
	-$(DEL) haribote.sys
