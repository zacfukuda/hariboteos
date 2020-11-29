MAKE = make -r
DEL = rm -f

default : 
	$(MAKE) img

ipl10.bin : ipl10.nas Makefile
	nasm ipl10.nas -o ipl10.bin -l ipl10.lst

haribote.sys : haribote.nas Makefile
	nasm haribote.nas -o haribote.sys -l haribote.lst

haribote.img : ipl10.bin haribote.sys Makefile
	mformat -f 1440 -C -B ipl10.bin -i haribote.img ::
	mcopy -i haribote.img haribote.sys ::

img :
	$(MAKE) haribote.img

run :
	$(MAKE) img
	qemu-system-i386 -drive file=haribote.img,format=raw,if=floppy -boot a

clean : 
	-$(DEL) ipl10.bin
	-$(DEL) ipl10.lst
	-$(DEL) haribote.sys
	-$(DEL) haribote.lst

src_only :
	$(MAKE) clean
	-$(DEL) haribote.img
