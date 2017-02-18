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
 * 17Feb2017 complete rewrite using double linked lists.
 * As each transpose is calculated it is removed from the Inlist
 * and added to a sublist.
 * Each sublist is added to the Groups list
 * 
 * 
 */

#include "symmetries.h"

#define STPTR(glistptr) ((SigTrans*)(glistptr->data))

#define USED(glistptr) (STPTR(glistptr)->found)

#define DEBUGSTOP {printf("\nDebug stop\n");exit(0);}

#define DEBUG 1

int main(int argc, char **argv)
{
	GList *InList = NULL, *source;
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
		InList = g_list_prepend(InList, stp);
		}
	} while(!(feof(fin)));	
	fclose(fin);
	
	// Set up the signatures and flags for each list entry
	source = InList;
	while(source != NULL) {
		stp = STPTR(source);	
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
	
	const int len_inlist = g_list_length(InList);
	printf("InList has %u elements.\n", len_inlist);
	
	GList *scan, *sublist, *Groups = NULL;
	SigTrans *data;
	while(InList != NULL) {
		data = STPTR(InList);
		sublist = NULL;		
		for(int abcd = 0; abcd < 2; ++abcd) {
			for(int bd = 0; bd < 4; ++bd) {
				for(int ef = 0; ef < 4; ++ef) {
					// find equal transpose
					scan = g_list_find_custom(InList, data, wrapper_equal_transpose);
					if(scan == NULL) {
						printf("1: Did not find transpose in InList.\n");
						exit(1);
					}
					// unlink equal transpose
					InList = g_list_remove_link(InList,scan);
					
					// save in sublist
					sublist = g_list_prepend(sublist, scan->data);
					
					// apply_ef rotation
					data = gt_apply_ef(data);				
					
				} // for ef...
				data = gt_apply_bd(data);
				if(data == NULL) {
					printf("bd returned NULL.\n");
					exit(1);
				} 			
			} // for bd...
			
			data = gt_apply_ac(data);
			
			for(int ef = 0; ef < 4; ++ef) {					
				// find equal transpose
				scan = g_list_find_custom(InList, data, wrapper_equal_transpose);
				if(scan == NULL) {
					printf("1: Did not find transpose in InList.\n");
					exit(1);
				}
					// unlink equal transpose
					InList = g_list_remove_link(InList,scan);
					
					// save in sublist
					sublist = g_list_prepend(sublist, scan->data);
					
					// apply_ef rotation
					data = gt_apply_ef(data);				
															
			} // for ef...
			
			data = gt_apply_ac(data);
			data = gt_apply_ac(data);
			
			for(int ef = 0; ef < 4; ++ef) {					
				// find equal transpose
				scan = g_list_find_custom(InList, data, wrapper_equal_transpose);
				if(scan == NULL) {
					printf("1: Did not find transpose in InList.\n");
					exit(1);
				}
				// unlink equal transpose
				InList = g_list_remove_link(InList,scan);
				
				// save in sublist
				sublist = g_list_prepend(sublist, scan->data);
				
				// apply_ef rotation
				data = gt_apply_ef(data);				
												
			} // for ef...
			
			// Restore initial config - have 24 configs
			data = gt_apply_ac(data);
			
			// reflect
			data = gt_apply_abcd(data);
		} // for abcd...
		
		Groups = g_list_prepend(Groups, sublist);
	}
	

	// output sublists
	sublist = Groups;
	while(sublist != NULL) {
		printf("Sublist has %u elements.\n", g_list_length(sublist->data));
		int index = 0;
		scan = sublist->data;	// scan becomes head of GList
		while(scan != NULL) {
			prt_sigtrans(STPTR(scan), index++);
			scan = g_list_next(scan);
		}
		sublist = g_list_next(sublist);
	}
	
	printf("Groups has %u elements.\n", g_list_length(Groups));
	
	printf("InList has %u elements.\n", g_list_length(InList));
		
	// cleanup code
	printf("Attempting to free memory.\n");
	sublist = Groups;
	while(sublist != NULL) {
		scan = sublist->data;	// scan is now head of GList
		while(scan != NULL) {
			// free(scan);
			scan = g_list_next(scan);
		}
		sublist = g_list_next(sublist);
	}
	g_list_free(Groups);
	
	return 0;
}

