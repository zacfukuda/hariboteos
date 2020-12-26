OBJS_API = api001.o api002.o api003.o api004.o api005.o api006.o api007.o api008.o api009.o api010.o api011.o api012.o api013.o api014.o api015.o api016.o api017.o api018.o api019.o api020.o api021.o api022.o api023.o api024.o api025.o api026.o api027.o alloca.o

MAKE = make -r
DEL = rm -f

default : 
	$(MAKE) libapi.a

# Library
# r: replace files existing inside the archive
# c: create a archive if not already existent
# s: create an object-file index into the archive
libapi.a : $(OBJS_API)
	i386-elf-ar rcs $@ $^

# NASM -> Object
# -g:generate debug, -f:format, -o:outfile, -l:listfile, elf:ELF32 (i386)
%.o : %.nas
	nasm -g -f elf $*.nas -o $*.o

# Remove files
clean : 
	-$(DEL) *.lst
	-$(DEL) *.o

src_only :
	$(MAKE) clean
	-$(DEL) libapi.a
