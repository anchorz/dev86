# set PATH to bin directory first

all: lib/libdos.a lib/libz.a

lib/libdos.a : obj obj/__mkargv.o
	-ar86 rv lib/libdos.a obj/__mkargv.o
	if [ -e lib/libdos.a_supersede ]  ; then mv lib/libdos.a_supersede lib/libdos.a; fi
	
lib/libz.a:  obj obj/crc32_asm.o #crc32.c is for reference only
	ar86 q $@ obj/crc32_asm.o
	
obj:
	mkdir obj
	
obj/%.o: src/%.c
#-Mf libc without -Mf
#-0 8086
#-W turn on assembler warning messages
#-ansi Pass the C source through unprotoize
	bcc -0 -c -W -o $@ -ansi -Ih/ -Ibcc_include/ $<

clean:
	rm -rf obj