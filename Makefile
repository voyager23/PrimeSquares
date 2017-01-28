# Makefile
# Normal format for gcc flags/options -o outfile infiles....

.PHONY : clean all

CFLAGS = -Wall -std=c11 -ggdb -Wno-unused-variable -O2

all : dat an ps bs hi

dat : dat_analysis.o utilities.o
	gcc $(CFLAGS) `pkg-config --cflags --libs glib-2.0 ` -o dat dat_analysis.o utilities.o 

ps : ps1.o utilities.o
	gcc $(CFLAGS) `pkg-config --cflags --libs glib-2.0 ` -o ps  utilities.o ps1.o

an : analysis.o utilities.o
	gcc $(CFLAGS) `pkg-config --cflags --libs glib-2.0 ` -o an analysis.o utilities.o 
	
bs : block_search.c ps1.h
	gcc $(CFLAGS)  `pkg-config --cflags --libs glib-2.0 ` -o bs block_search.c
	
# ----------------------------------------------------------------------
ps1.o : ps1.c ps1.h
	gcc $(CFLAGS) `pkg-config --cflags --libs glib-2.0 ` -c ps1.c 
	
analysis.o : analysis.c ps1.h
	gcc $(CFLAGS) `pkg-config --cflags --libs glib-2.0 ` -c analysis.c 
	
dat_analysis.o : dat_analysis.c ps1.h
	gcc $(CFLAGS) `pkg-config --cflags --libs glib-2.0 ` -c dat_analysis.c 	
	
utilities.o : utilities.c ps1.h
	gcc $(CFLAGS) `pkg-config --cflags --libs glib-2.0 ` -c utilities.c 
	

#-----------------------------------------------------------------------	
		
clean :
	rm ps1 ps1.o an1 analysis.o utilities.o
