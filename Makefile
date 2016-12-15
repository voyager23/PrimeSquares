# Makefile

objects = ps1.o

ps : $(objects)
	gcc `pkg-config --cflags --libs glib-2.0` -o ps1 $(objects)
	
ps1.o : ps1.c
	gcc -c `pkg-config --cflags --libs glib-2.0` ps1.c	
	
.PHONY : clean		
clean :
	rm ps1 $(objects)
