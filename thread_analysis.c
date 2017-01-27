/*
 * thread_analysis.c
 * 
 * Copyright 2017 mike <mike@localhost.localdomain>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include "ps1.h"
#include <pthread.h>

typedef struct td {
	int thread_id;
	int nRows;
	gpRow *pRow;
}ThreadData;

GSList *DataBlocks = NULL;

int main(int argc, char **argv)
{	
	int in_rows;
	ThreadData *pThreadData;
	
	// Working file
	char *infile="equalsums.dat";
	
	FILE *fin = fopen(infile,"rb");
	if(fin == NULL) {
		printf("Could not open %s \n", infile);
		exit(1);
	
	do {
		fread(&in_rows, sizeof(int), 1, fin);
		if(feof(fin)) break;
		pThreadData = (ThreadData*)malloc(sizeof(ThreadData));
		pThreadData->thread_id = -1;
		pThreadData->nRows = in_rows;
		pThreadData->pRow = (gpRow*)malloc(sizeof(gprime) * 4 * in_rows);
		
		

}

