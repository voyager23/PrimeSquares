/*
 * list_toctas.c
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

#include "symmetries.h"

#define STPTR(a) ((SigTrans*)(a->data))

#define DEBUGSTOP {printf("\nDebug stop\n");exit(0);}

#define DEBUG 1

int main(int argc, char **argv)
{
	GSList *InList = NULL, *source;
	SigTrans *stp;
	FILE *fin;
		
	if(argc == 2) {
		fin = fopen(argv[1],"rb");
	} else {
		fin = fopen("blocks/Tocta-9-31.blk","rb");
	}
	if(fin == NULL) {
		printf("Unable to open %s ... stopping.\n", argv[1]);
		exit(1);
	}
	do {
		stp = (SigTrans*)malloc(sizeof(SigTrans));
		fread(&(stp->transpose), sizeof(Matrix), 1, fin);
		if(feof(fin)) {
			free(stp);
		} else {
		InList = g_slist_prepend(InList, stp);
		}
	} while(!(feof(fin)));	
	fclose(fin);
	
	// Set up the signatures and flags for each list entry
	source = InList;
	while(source != NULL) {
		stp = source->data;		
		// read 12 gprimes from the transpose into sig_major
		int sig_idx = 0;
		for(int row = 0; row < 4; ++row) {
			for(int col = 1; col < 4; ++col) {
				stp->sig_minor[sig_idx] = CMPLX(0,0);	// clear the minor signature
				stp->sig_major[sig_idx] = stp->transpose[row][col];	// set major signature
				sig_idx++;
			} // for col...
		} // for row...
		// setup sig_minor
		sig_idx = 0;
		for(int row = 0; row < 4; ++row) stp->sig_minor[sig_idx++] = stp->transpose[row][0];		
		// qsort major signature (msb first)
		qsort((void*)stp->sig_major, 12, sizeof(gprime), qsort_signature_compare);		
		// qsort minor signature (msb first)
		qsort((void*)stp->sig_minor, 12, sizeof(gprime), qsort_signature_compare);		
		// clear 'found' flag
		stp->found = false;		
		// next SigTrans
		source = source->next;
	} // while source....
	
	const int len_inlist = g_slist_length(InList);
	printf("InList has %u elements.\n", len_inlist);
	
	// Add InList integrity checks here???
	// All sig_major must be equal
	// other tests .....
	// debug exit
	
	GSList *scan, *Groups = NULL;

	SigTrans *data;
	
	// loop start
	do {
		source = InList;
		while(source != NULL) {
			if(STPTR(source)->found != true) break;
			source = g_slist_next(source);
		}
		if(source != NULL) {
			data = STPTR(source);
			// data points to unused config
			// main loop - expects a SigTrans *data to base configuration
			GSList *sublist = (GSList*)malloc(sizeof(GSList*));
			sublist = NULL;
			int index = 0;
			for(int abcd = 0; abcd < 2; ++abcd) {
				for(int bd = 0; bd < 4; ++bd) {
					for(int ef = 0; ef < 4; ++ef) {
						data = gt_apply_ef(data);
						if(data == NULL) {
							printf("ef returned NULL.\n");
							exit(1);
						}			
						if(DEBUG)
							prt_sigtrans(data, index++ );	// optional printout
						scan = g_slist_find_custom(InList, data, wrapper_equal_transpose);
						if(scan == NULL) {
							printf("1: Did not find transpose in InList.\n");
							exit(1);
						}
						// mark as found
						STPTR(scan)->found = true;						
					} // for ef...
					data = gt_apply_bd(data);
					if(data == NULL) {
						printf("bd returned NULL.\n");
						exit(1);
					} 			
				} // for bd...
				
				data = gt_apply_ac(data);
				
				for(int ef = 0; ef < 4; ++ef) {					
					data = gt_apply_ef(data);
					if(data == NULL) {
						printf("ef returned NULL.\n");
						exit(1);
					}			
					if(DEBUG)
						prt_sigtrans(data, index++ );	// optional printout
					scan = g_slist_find_custom(InList, data, wrapper_equal_transpose);
					if(scan == NULL) {
						printf("2: Did not find transpose in InList.\n");
						exit(1);
					}
					// mark as found
					STPTR(scan)->found = true;						
				} // for ef...
				
				data = gt_apply_ac(data);
				data = gt_apply_ac(data);
				
				for(int ef = 0; ef < 4; ++ef) {					
						data = gt_apply_ef(data);
						if(data == NULL) {
							printf("ef returned NULL.\n");
							exit(1);
						}			
						if(DEBUG)
							prt_sigtrans(data, index++ );	// optional printout
						scan = g_slist_find_custom(InList, data, wrapper_equal_transpose);
						if(scan == NULL) {
							printf("3: Did not find transpose in InList.\n");
							exit(1);
						}
						// mark as found
						STPTR(scan)->found = true;							
				} // for ef...
				
				// Restore initial config - have 24 configs
				data = gt_apply_ac(data);
				
				// reflect
				data = gt_apply_abcd(data);
			} // for abcd...
			
			// save sublist to groups
			Groups = g_slist_prepend(Groups, sublist);
			
		} // if source ..
		
		// print out all of InList
		// todo
		// print out length of Groups
		printf("\nLength of Groups = %u\n", g_slist_length(Groups));
		GSList *grps = Groups;
		GSList *sublist = grps->data;
		int index = 0;		
		while(sublist != NULL) {
			prt_sigtrans(STPTR(sublist),index++);
			sublist = g_slist_next(sublist);
		}
		
		// =====DEBUGSTOP=====
		// DEBUGSTOP
		
	} while (source != NULL);
	
	
	

	// cleanup code
	g_slist_free(InList);
	return 0;
}

