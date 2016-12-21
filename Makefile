# Makefile

objects = ps1.o

ps : $(objects)
	gcc `pkg-config --cflags --libs glib-2.0` -o ps1 $(objects)
	
	
ps1.o : ps1.c ps1.h
	gcc -c `pkg-config --cflags --libs glib-2.0` ps1.c
	
bs : block_search.c ps1.h
	gcc `pkg-config --cflags --libs glib-2.0` -o bs1 block_search.c
	
.PHONY : clean		
clean :
	rm ps1 $(objects)
