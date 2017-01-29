# Makefile
# Normal format for gcc flags/options -o outfile infiles....
# gcc on Mint18 needs source files first

.PHONY : clean all

CFLAGS = -Wall -std=c11 -ggdb -Wno-unused-variable -Wno-unused-result -O2 -mtune=native

all : dat an ps bs ta

dat : dat_analysis.o utilities.o
	gcc dat_analysis.o utilities.o $(CFLAGS) `pkg-config --cflags --libs glib-2.0 ` -o dat 

ps : ps1.o utilities.o
	gcc ps1.o utilities.o $(CFLAGS) `pkg-config --cflags --libs glib-2.0 ` -o ps  

an : analysis.o utilities.o
	gcc analysis.o utilities.o $(CFLAGS) `pkg-config --cflags --libs glib-2.0 ` -o an  
	
bs : block_search.c ps1.h
	gcc block_search.c $(CFLAGS)  `pkg-config --cflags --libs glib-2.0 ` -o bs
	
ta : thread_analysis.c  search_routine.c
	gcc thread_analysis.c search_routine.c -pthread $(CFLAGS) `pkg-config --cflags --libs glib-2.0 ` -o ta 
	
# ----------------------------------------------------------------------
ps1.o : ps1.c ps1.h
	gcc -c ps1.c $(CFLAGS) `pkg-config --cflags --libs glib-2.0 ` 
	
analysis.o : analysis.c ps1.h
	gcc -c analysis.c $(CFLAGS) `pkg-config --cflags --libs glib-2.0 ` 
	
dat_analysis.o : dat_analysis.c ps1.h
	gcc -c dat_analysis.c $(CFLAGS) `pkg-config --cflags --libs glib-2.0 ` 
	
utilities.o : utilities.c ps1.h
	gcc -c utilities.c $(CFLAGS) `pkg-config --cflags --libs glib-2.0 ` 
	

#-----------------------------------------------------------------------	
		
clean :
	rm ps1 ps1.o an1 analysis.o utilities.o
