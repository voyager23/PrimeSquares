# Makefile
# Normal format for gcc flags/options -o outfile infiles....

CFLAGS = -Wall -std=c11 -ggdb -Wno-unused-variable

ps : ps1.o utilities.o
	gcc $(CFLAGS) `pkg-config --cflags --libs glib-2.0 ` -o ps1 ps1.o utilities.o

an1 : analysis.o utilities.o
	gcc $(CFLAGS) `pkg-config --cflags --libs glib-2.0 ` -o an1 analysis.o utilities.o 
	
bs : block_search.c ps1.h
	gcc $(CFLAGS)  `pkg-config --cflags --libs glib-2.0 ` -o bs1 block_search.c
	
# ----------------------------------------------------------------------
ps1.o : ps1.c ps1.h
	gcc $(CFLAGS) `pkg-config --cflags --libs glib-2.0 ` -c ps1.c 
	
analysis.o : analysis.c ps1.h
	gcc $(CFLAGS) `pkg-config --cflags --libs glib-2.0 ` -c analysis.c 
	
utilities.o : utilities.c ps1.h
	gcc $(CFLAGS) `pkg-config --cflags --libs glib-2.0 ` -c utilities.c 
	

#-----------------------------------------------------------------------	
.PHONY : clean		
clean :
	rm ps1 ps1.o an1 analysis.o utilities.o
