# Makefile

CFLAGS = -Wall -std=c11 -ggdb -Wno-unused-variable

an1 : analysis.o utilities.o
	gcc $(CFLAGS) -o an1 analysis.o utilities.o `pkg-config --cflags --libs glib-2.0 ` 

ps : ps1.o
	gcc $(CFLAGS) -o ps1 ps1.o `pkg-config --cflags --libs glib-2.0 ` 
	
bs : block_search.c ps1.h
	gcc $(CFLAGS) -o bs1 block_search.c `pkg-config --cflags --libs glib-2.0 ` 
	
# ----------------------------------------------------------------------
ps1.o : ps1.c ps1.h
	gcc $(CFLAGS) -c ps1.c `pkg-config --cflags --libs glib-2.0 ` 
	
analysis.o : analysis.c ps1.h
	gcc $(CFLAGS) -c analysis.c `pkg-config --cflags --libs glib-2.0 ` 
	
utilities.o : utilities.c ps1.h
	gcc $(CFLAGS) -c utilities.c `pkg-config --cflags --libs glib-2.0 ` 
	

#-----------------------------------------------------------------------	
.PHONY : clean		
clean :
	rm ps1 ps1.o an1 analysis.o
