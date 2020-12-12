OBJS_BOOTPACK = bootpack.o naskfunc.o hankaku.o graphic.o dsctbl.o mysprintf.o myfunction.o int.o fifo.o keyboard.o mouse.o memory.o sheet.o timer.o mtask.o window.o console.o file.o

IMG_REQUISITE = ipl10.bin haribote.sys hello.hrb hello2.hrb a.hrb hello3.hrb crack1.hrb crack2.hrb crack3.hrb crack4.hrb crack5.hrb crack6.hrb
IMG_COPY = haribote.sys ipl10.nas make.bat hello.hrb hello2.hrb a.hrb hello3.hrb crack1.hrb crack2.hrb crack3.hrb crack4.hrb crack5.hrb crack6.hrb

MAKE = make -r
DEL = rm -f

CC = i386-elf-gcc
CFLAGS = -m32 -fno-builtin
COPTION = -march=i486 -nostdlib
COSLD = -T hrb.ld
CAPPLD = -T app.ld

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
hello.hrb : hello.nas
	nasm $< -o $@ -l hello.lst

hello2.hrb : hello2.nas
	nasm $< -o $@ -l hello2.lst

a.hrb : a.o a_nask.o app.ld 
	$(CC) $(CFLAGS) $(COPTION) $(CAPPLD) -g a.o a_nask.o -o $@

hello3.hrb : hello3.o a_nask.o app.ld 
	$(CC) $(CFLAGS) $(COPTION) $(CAPPLD) -g hello3.o a_nask.o -o $@

crack1.hrb : crack1.o a_nask.o app.ld
	$(CC) $(CFLAGS) $(COPTION) $(CAPPLD) -g crack1.o a_nask.o -o $@

crack2.hrb : crack2.nas
	nasm $< -o $@ -l crack2.lst

crack3.hrb : crack3.nas
	nasm $< -o $@ -l crack3.lst

crack4.hrb : crack4.nas
	nasm $< -o $@ -l crack4.lst

crack5.hrb : crack5.nas
	nasm $< -o $@ -l crack5.lst

crack6.hrb : crack6.nas
	nasm $< -o $@ -l crack6.lst

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
	nasm -g -f elf $*.nas -o $*.o -l $*.lst

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
