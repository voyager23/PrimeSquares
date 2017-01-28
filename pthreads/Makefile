#Makefile

CFLAGS = -Wall -std=c11 -ggdb -Wno-unused-variable -O2

ta : thread_analysis.c ../ps1.h search_routine.c
	gcc -pthread $(CFLAGS) `pkg-config --cflags --libs glib-2.0 ` -o ta thread_analysis.c search_routine.c

hi : hello_arg2.c
	gcc -pthread -o hi hello_arg2.c
