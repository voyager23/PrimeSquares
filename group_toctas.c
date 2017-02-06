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

int equal_transpose(SigTrans *stpa, SigTrans *stpb) {
	// transpose is a 4*4 matrix of gprime
	// testing for equal gprimes in corresponding locations
	// Returns 0 - no error - if transpose matrixes are equal
	int rv = 1;
	int idx;
	gprime *gpa = (gprime*)&(stpa->transpose);
	gprime *gpb = (gprime*)&(stpb->transpose);
	for(idx = 0; idx < 16; ++idx) {
		printf("%d\n", idx);
		rv = compare_gprime((gpa + idx), (gpb + idx));
		if(rv != 0) break;
	}
	return rv;
}

// =====================================================================

int main(int argc, char **argv)
{
	char *fname26 = "blocks/Tocta-12-26.blk";	// 144 configs (3 groups)
	char *fname28 = "blocks/Tocta-12-28.blk";	// 192 configs (4 groups)
	char *fname31 = "blocks/Tocta-17-31.blk";	// 
	char *fname35 = "blocks/Tocta-14-36.blk";	//
	
	GSList *working, *inlist = NULL, *outlist = NULL;
	SigTrans *st;
	FILE *fin;
	
	fin = fopen(fname35,"rb");
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
	SigTrans *stp, *stpa;
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
	const int len_inlist = g_slist_length(inlist);
	printf("Inlist has %u elements.\n", len_inlist);
	
	// New Code based on sorting each SigTrans into an output list.
	// Each SigTrans in the same sublist will have exactly matching signatures.	
	
	GSList *ListOfLists = NULL, *lp;
	GSList *head;
	// Read a series of SigTrans from inlist and assign to sublists headed in ListOfLists
	working = inlist;
	while(working != NULL) {
		stp = working->data;
		// distribute SigTrans
		lp = ListOfLists;
		while(lp != NULL) {
			GSList* head = lp->data;
			stpa = head->data;
			// test sigs here
			gprime *pa = (gprime*)&stp->signature;
			gprime *pb = (gprime*)&stpa->signature;
			int match = 1;
			for(int x = 0; x < 12; ++x) {
				if ( compare_gprime( pa + x, pb + x) != 0 ) {
					match = 0;
					break;
				}
			}
			if( match ) {
				head = g_slist_prepend(head, stp); // add entry to this sublist
				lp->data = head;
				break;
			} // if...
			lp = g_slist_next(lp);
		}
		if(lp == NULL) { // no matches found, add new sublist to ListOfLists
			head = NULL;
			head = g_slist_prepend(head,stp);
			ListOfLists = g_slist_prepend(ListOfLists,head);
		}
		// end distribute SigTrans	
		working = g_slist_next(working);
	}
	
	// publish the sublist lengths
	lp = ListOfLists;
	int count = 0;
	while(lp != NULL) {
		++count;
		int length = g_slist_length(lp->data);
		if (length > 48)
			printf("Sublist:%d		length: %u\n", count, g_slist_length(lp->data));
		lp = g_slist_next(lp);
	}
	printf("Count %d\n",count);
		
	// Cleanup code
	printf("\n");
	g_slist_free_full(inlist, free);
	return 0;
}

