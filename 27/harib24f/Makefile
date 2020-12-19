OBJS_BOOTPACK = bootpack.o naskfunc.o hankaku.o graphic.o dsctbl.o mysprintf.o myfunction.o int.o fifo.o keyboard.o mouse.o memory.o sheet.o timer.o mtask.o window.o console.o file.o

OBJS_API = api001.o api002.o api003.o api004.o api005.o api006.o api007.o api008.o api009.o api010.o api011.o api012.o api013.o api014.o api015.o api016.o api017.o api018.o api019.o api020.o

IMG_REQUISITE = ipl10.bin haribote.sys a.hrb hello3.hrb hello4.hrb hello5.hrb winhelo.hrb winhelo2.hrb winhelo3.hrb star1.hrb stars.hrb stars2.hrb lines.hrb walk.hrb noodle.hrb beepdown.hrb color.hrb color2.hrb
IMG_COPY = haribote.sys ipl10.nas make.bat a.hrb hello3.hrb hello4.hrb hello5.hrb winhelo.hrb winhelo2.hrb winhelo3.hrb star1.hrb stars.hrb stars2.hrb lines.hrb walk.hrb noodle.hrb beepdown.hrb color.hrb color2.hrb

MAKE = make -r
DEL = rm -f

CC = i386-elf-gcc
CFLAGS = -m32 -fno-builtin
COPTION = -march=i486 -nostdlib
COSLD = -T hrb.ld
CAPPLD = -T app.ld
CAPPLD2 = -T app2.ld
CC_WITH_OPTION = i386-elf-gcc -m32 -march=i486 -nostdlib
# -m32: 32-bit environment
# -fno-builtin: ignore built-in functions that don't begin with __builtin_ as prefix
# -march=i486: CPU type
# -nostdlib: Do not use the standard system startup files or libraries when linking.

default : 
	$(MAKE) img

# Generate files
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

haribote.sys : asmhead.bin bootpack.hrb Makefile
	cat asmhead.bin bootpack.hrb > haribote.sys

# Applications
# libapi.a : api001.o api004.o
libapi.a : $(OBJS_API)
	i386-elf-ar rcs $@ $^
# r: replace files existing inside the archive
# c: create a archive if not already existent
# s: create an object-file index into the archive

hello5.hrb : hello5.o app.ld
	$(CC_WITH_OPTION) $(CAPPLD) -o $@ $<

noodle.hrb : noodle.o mysprintf.o libapi.a app2.ld
	$(CC_WITH_OPTION) $(CAPPLD2) -o $@ $< mysprintf.o libapi.a

# Disc Image
# mtool: https://www.gnu.org/software/mtools/manual/html_node/mformat.html
# -f: size, -C: hidden_sectors, -B: boot_sector, -i: 
haribote.img : $(IMG_REQUISITE) Makefile
	mformat -f 1440 -C -B ipl10.bin -i haribote.img ::
	mcopy -i haribote.img $(IMG_COPY) ::

# Rule
%.o : %.c
	$(CC) $(CFLAGS) -c $*.c -o $*.o

# -g:generate debug, -f:format, -o:outfile, -l:listfile, elf:ELF32 (i386)
%.o : %.nas
	nasm -g -f elf $*.nas -o $*.o

# Application
%.hrb : %.o libapi.a app.ld
	$(CC_WITH_OPTION) $(CAPPLD) -o $@ $< libapi.a

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
	-$(DEL) *.map
	-$(DEL) hankaku.c
	-$(DEL) convHankakuTxt

src_only :
	$(MAKE) clean
	-$(DEL) haribote.img
