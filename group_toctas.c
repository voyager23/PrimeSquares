/*
 * group_toctas.c
 * 
 * Copyright 2017 mike <mike@sedawk>
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

// =====================================================================

#include "ps1.h"

typedef struct sigtrans {
	Signature signature;
	Matrix transpose;
}SigTrans;

int qsort_signature_compare(gconstpointer a, gconstpointer b);
int qsort_sig_wrapper(gconstpointer a, gconstpointer b);

int qsort_sig_wrapper(gconstpointer a, gconstpointer b) {	
	// The wrapper receives the data from each list element in the form of a gconstpointer.
	// This points to a SigTrans structure.
	// The qsort_signature_compare function parameters are assumed to be Signature*.
	// 1) Get pointers to the signature
	// 2) Cast pointers and call qsort_signature_compare
	// 3) Return result
	
	Signature *siga = &(((SigTrans*)a)->signature);
	Signature *sigb = &(((SigTrans*)b)->signature);
	
	return( qsort_signature_compare( (gconstpointer)siga, (gconstpointer)sigb ) );	
}

int qsort_signature_compare(gconstpointer a, gconstpointer b) {
	// Require MSB first
	// if *a > *b return -1, if *a < *b return +1 else return 0
	// We assume a and b can be cast to Signature*.
	
	Signature *pa = (Signature*)a;
	Signature *pb = (Signature*)b;
	int rv;
	for(int idx = 0; idx < 12; ++idx) {
		rv = compare_gprime((gconstpointer)pa++, (gconstpointer)pb++);
		// if a < b rv = -1 so invert return value
		if(rv != 0) return( -(rv) );
	}
	return 0;
}

// =====================================================================

int main(int argc, char **argv)
{
	char *fname26 = "blocks/Tocta-12-26.blk";	// 144 configs (3 groups)
	char *fname28 = "blocks/Tocta-12-28.blk";	// 192 configs (4 groups)
	GSList *working, *inlist = NULL, *outlist = NULL;
	SigTrans *st;
	FILE *fin;
	
	fin = fopen(fname28,"rb");
	if(fin == NULL) {
		printf("Unable to open %s ... stopping.\n", fname26);
		exit(1);
	}
	
	do {
		st = (SigTrans*)malloc(sizeof(SigTrans));
		fread(&(st->transpose), sizeof(Matrix), 1, fin);
		if(feof(fin)) {
			free(st);
		} else {
		inlist = g_slist_prepend(inlist, st);
		}
	} while(!(feof(fin)));	
	fclose(fin);

	working = inlist;
	SigTrans *stp;
	while(working != NULL) {
		stp = working->data;
		// read 12 gprimes from the transpose in signature
		int sig_idx = 0;
		for(int row = 0; row < 4; ++row) {
			for(int col = 1; col < 4; ++col) {
				stp->signature[sig_idx++] = stp->transpose[row][col];
			}
		}	
		// qsort signature (msb first)
		qsort((void*)stp->signature, 12, sizeof(gprime), qsort_signature_compare);
		// next
		working = working->next;
	}
	
	printf("Inlist has %u elements.\n", g_slist_length(inlist));
	// Sort the inlist by signature
	inlist = g_slist_sort(inlist, qsort_sig_wrapper);
	printf("Inlist has %u elements.\n", g_slist_length(inlist));
	
	working = inlist;
	while(working != NULL) {
		stp = working->data;
		for(int x = 0; x < 12; ++x) prt_gprime( stp->signature[x] );
		printf("\n");
		working = g_slist_next(working);
	}
	
	// Cleanup code
	printf("\n");
	g_slist_free_full(inlist, free);
	return 0;
}

