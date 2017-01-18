# Makefile

CFLAGS = -Wall -std=c11 -ggdb -Wno-unused-variable

an1 : analysis.o utilities.o
	gcc $(CFLAGS) `pkg-config --cflags --libs glib-2.0 gsl` -o an1 analysis.o utilities.o

ps : ps1.o
	gcc $(CFLAGS) `pkg-config --cflags --libs glib-2.0 gsl` -o ps1 ps1.o
	
bs : block_search.c ps1.h
	gcc $(CFLAGS) `pkg-config --cflags --libs glib-2.0 gsl` -o bs1 block_search.c
	
# ----------------------------------------------------------------------
ps1.o : ps1.c ps1.h
	gcc $(CFLAGS) -c `pkg-config --cflags --libs glib-2.0 gsl` ps1.c
	
analysis.o : analysis.c ps1.h
	gcc $(CFLAGS) -c `pkg-config --cflags --libs glib-2.0 gsl` analysis.c
	
utilities.o : utilities.c ps1.h
	gcc $(CFLAGS) -c `pkg-config --cflags --libs glib-2.0 gsl` utilities.c
	

#-----------------------------------------------------------------------	
.PHONY : clean		
clean :
	rm ps1 ps1.o an1 analysis.o
