MAKE = make -r
DEL = rm -f

CC = i386-elf-gcc
CFLAGS = -m32 -fno-builtin
COPTION = -march=i486 -nostdlib
AR = i386-elf-ar

default :
	$(MAKE) libstdio.a
	$(MAKE) libstring.a

libstdio.a : sprintf.o
	$(AR) rcs $@ $^

libstring.a : strcmp.o strncmp.o memcmp.o
	$(AR) rcs $@ $^

# C -> Object
%.o : %.c
	$(CC) $(CFLAGS) -c $*.c -o $*.o

# Remove
clean :
	-$(DEL) *.o

src_only :
	$(MAKE) clean
	-$(DEL) *.a
