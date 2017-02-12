# Makefile
# Normal format for gcc flags/options -o outfile infiles....
# gcc on Mint18 needs source files first

.PHONY : clean all

CFLAGS = -Wall -std=c11 -ggdb -Wno-unused-variable -Wno-unused-result 

all : sym

sym : symmetries.o lib_sym.o ../ps1.h
	gcc symmetries.o lib_sym.o ../utilities.o $(CFLAGS) `pkg-config --cflags --libs glib-2.0 ` -o sym

# ----------------------------------------------------------------------

symmetries.o : symmetries.c lib_sym.c ../ps1.h
	gcc -c symmetries.c $(CFLAGS) `pkg-config --cflags --libs glib-2.0 `
	
	
lib_sym.o : lib_sym.c symmetries.c ../ps1.h
	gcc -c lib_sym.c $(CFLAGS) `pkg-config --cflags --libs glib-2.0 `

#-----------------------------------------------------------------------	
		
clean :
	rm sym *.o
