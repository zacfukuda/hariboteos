OBJS_BOOTPACK = bootpack.o naskfunc.o hankaku.o graphic.o dsctbl.o mysprintf.o myfunction.o int.o fifo.o keyboard.o mouse.o memory.o sheet.o timer.o mtask.o window.o console.o file.o

IMG_REQUISITE = ipl10.bin haribote.sys hello.hrb hello2.hrb a.hrb hello3.hrb hello4.hrb hello5.hrb winhelo.hrb winhelo2.hrb winhelo3.hrb star1.hrb stars.hrb stars2.hrb
IMG_COPY = haribote.sys ipl10.nas make.bat hello.hrb hello2.hrb a.hrb hello3.hrb hello4.hrb hello5.hrb winhelo.hrb winhelo2.hrb winhelo3.hrb star1.hrb stars.hrb stars2.hrb

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

hello4.hrb : hello4.o a_nask.o app.ld 
	$(CC) $(CFLAGS) $(COPTION) $(CAPPLD) -g hello4.o a_nask.o -o $@

hello5.hrb : hello5.o app.ld
	$(CC) $(CFLAGS) $(COPTION) $(CAPPLD) -g hello5.o -o $@

winhelo.hrb : winhelo.o a_nask.o app.ld
	$(CC) $(CFLAGS) $(COPTION) $(CAPPLD) -g winhelo.o a_nask.o -o $@

winhelo2.hrb : winhelo2.o a_nask.o app.ld
	$(CC) $(CFLAGS) $(COPTION) $(CAPPLD) -g winhelo2.o a_nask.o -o $@

winhelo3.hrb : winhelo3.o a_nask.o app.ld
	$(CC) $(CFLAGS) $(COPTION) $(CAPPLD) -g winhelo3.o a_nask.o -o $@

star1.hrb : star1.o a_nask.o app.ld
	$(CC) $(CFLAGS) $(COPTION) $(CAPPLD) -g star1.o a_nask.o -o $@

stars.hrb : stars.o a_nask.o app.ld
	$(CC) $(CFLAGS) $(COPTION) $(CAPPLD) -g stars.o a_nask.o -o $@

stars2.hrb : stars2.o a_nask.o app.ld
	$(CC) $(CFLAGS) $(COPTION) $(CAPPLD) -g stars2.o a_nask.o -o $@

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
