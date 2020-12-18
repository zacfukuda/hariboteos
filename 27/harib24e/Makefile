OBJS_BOOTPACK = bootpack.o naskfunc.o hankaku.o graphic.o dsctbl.o mysprintf.o myfunction.o int.o fifo.o keyboard.o mouse.o memory.o sheet.o timer.o mtask.o window.o console.o file.o

OBJS_API = api001.o api002.o api003.o api004.o api005.o api006.o api007.o api008.o api009.o api010.o api011.o api012.o api013.o api014.o api015.o api016.o api017.o api018.o api019.o api020.o

IMG_REQUISITE = ipl10.bin haribote.sys hello.hrb hello2.hrb a.hrb hello3.hrb hello4.hrb hello5.hrb winhelo.hrb winhelo2.hrb winhelo3.hrb star1.hrb stars.hrb stars2.hrb lines.hrb walk.hrb noodle.hrb beepdown.hrb color.hrb color2.hrb crack7.hrb
IMG_COPY = haribote.sys ipl10.nas make.bat hello.hrb hello2.hrb a.hrb hello3.hrb hello4.hrb hello5.hrb winhelo.hrb winhelo2.hrb winhelo3.hrb star1.hrb stars.hrb stars2.hrb lines.hrb walk.hrb noodle.hrb beepdown.hrb color.hrb color2.hrb crack7.hrb

MAKE = make -r
DEL = rm -f

CC = i386-elf-gcc
CFLAGS = -m32 -fno-builtin
COPTION = -march=i486 -nostdlib
COSLD = -T hrb.ld
CAPPLD = -T app.ld
CAPPLD2 = -T app2.ld
CC_WITH_OPTION = $(CC) $(CFLAGS) $(COPTION)

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

a.hrb : a.o $(OBJS_API) app.ld 
	$(CC_WITH_OPTION) $(CAPPLD) -g a.o $(OBJS_API) -o $@

hello3.hrb : hello3.o $(OBJS_API) app.ld 
	$(CC_WITH_OPTION) $(CAPPLD) -g hello3.o $(OBJS_API) -o $@

hello4.hrb : hello4.o $(OBJS_API) app.ld 
	$(CC_WITH_OPTION) $(CAPPLD) -g hello4.o $(OBJS_API) -o $@

hello5.hrb : hello5.o app.ld
	$(CC_WITH_OPTION) $(CAPPLD) -g hello5.o -o $@

winhelo.hrb : winhelo.o $(OBJS_API) app.ld
	$(CC_WITH_OPTION) $(CAPPLD) -g winhelo.o $(OBJS_API) -o $@

winhelo2.hrb : winhelo2.o $(OBJS_API) app.ld
	$(CC_WITH_OPTION) $(CAPPLD) -g winhelo2.o $(OBJS_API) -o $@

winhelo3.hrb : winhelo3.o $(OBJS_API) app.ld
	$(CC_WITH_OPTION) $(CAPPLD) -Os -fdata-sections -ffunction-sections -g winhelo3.o $(OBJS_API) -o $@ -Wl,--gc-sections

star1.hrb : star1.o $(OBJS_API) app.ld
	$(CC_WITH_OPTION) $(CAPPLD) -g star1.o $(OBJS_API) -o $@

stars.hrb : stars.o $(OBJS_API) app.ld
	$(CC_WITH_OPTION) $(CAPPLD) -g stars.o $(OBJS_API) -o $@

stars2.hrb : stars2.o $(OBJS_API) app.ld
	$(CC_WITH_OPTION) $(CAPPLD) -g stars2.o $(OBJS_API) -o $@

lines.hrb : lines.o $(OBJS_API) app.ld
	$(CC_WITH_OPTION) $(CAPPLD) -g lines.o $(OBJS_API) -o $@

walk.hrb : walk.o $(OBJS_API) app.ld
	$(CC_WITH_OPTION) $(CAPPLD) -g walk.o $(OBJS_API) -o $@

noodle.hrb : noodle.o $(OBJS_API) mysprintf.o app.ld
	$(CC_WITH_OPTION) $(CAPPLD2) -g noodle.o $(OBJS_API) mysprintf.o -o $@

beepdown.hrb : beepdown.o $(OBJS_API) app.ld
	$(CC_WITH_OPTION) $(CAPPLD) -g beepdown.o $(OBJS_API) -o $@

color.hrb : color.o $(OBJS_API) app.ld
	$(CC_WITH_OPTION) $(CAPPLD) -g color.o $(OBJS_API) -o $@

color2.hrb : color2.o $(OBJS_API) app.ld
	$(CC_WITH_OPTION) $(CAPPLD2) -g color2.o $(OBJS_API) -o $@

crack7.hrb : crack7.o app.ld
	$(CC_WITH_OPTION) $(CAPPLD) -g crack7.o -o $@

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
