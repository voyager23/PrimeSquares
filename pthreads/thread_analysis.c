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

#include "../ps1.h"
#include <unistd.h>

void *search_routine(void *p);

ThreadData *thread_data_array;

int main(int argc, char **argv)
{
	// Working file
	char *outfile="equalsums.dat";
	
	int nBlocks, nRows, block, rc;
	gpRow *equalsums;	
	
	FILE *fin = fopen(outfile,"rb");
	if(fin == NULL) {
		printf("Error: Failed to open data file for reading.\n");
		exit(1);
	}
	
	// block count code here
	equalsums = (gpRow*)malloc(sizeof(gprime)*4);
	nBlocks = 0;
	while(!(feof(fin))) {
		fread(&nRows, sizeof(int), 1, fin);
		if(feof(fin)) break;
		++nBlocks;
		for(int b = 0; b < nRows; ++b) 
			fread(equalsums, sizeof(gprime), 4, fin);  // read and discard rows
	}	
	free(equalsums);
	// end block count	
	printf("Found %d blocks\n", nBlocks);
	
	thread_data_array = (ThreadData*)malloc(sizeof(ThreadData) * nBlocks);
	
	// Rewind and read the data file, allocating and populating blocks of data
	// for each thread. Save values in each ThreadData structure;
	rewind(fin);
	block = 0;	
	do {
		fread(&nRows, sizeof(int), 1, fin);
		if(!feof(fin)) {
			ThreadData *tdp = (thread_data_array + block);
			// set rows
			tdp->nRows = nRows;
			// allocate memory
			tdp->row_ptr = (gpRow*)malloc(sizeof(gprime)*4*nRows);
			// populate with rows of gprimes
			fread(tdp->row_ptr, sizeof(gprime), 4*nRows, fin);
			tdp->nToctas = -1;
			tdp->running = -1;
			tdp->idx = block;
			printf("Launching block %i\n", block);
			// launch thread here
			rc = pthread_create(&(tdp->thread_id), NULL, search_routine, (void*)tdp);
			if (rc) {
				printf("ERROR; return code from pthread_create() is %d\n", rc);
				exit(-1);
			}
			++block;
		}
	} while(!(feof(fin)));
	
	// join threads here
	
	sleep(5);
	
	// output any results	
	
	//-----Cleanup-----
	for(int b = 0; b < nBlocks; ++b) free((thread_data_array + b)->row_ptr);
	free(thread_data_array);
	return 0;
}

