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
 * 18Feb2017 Try using arrays instead of lists, developed on use_arrays
 * branch.
 * Executable is ./lta
 * 
 * 
 */

#include "symmetries.h"

#define DEBUGSTOP {printf("\nDebug stop\n");exit(0);}
#define DEBUG 1
#define REFROTS 48

int main(int argc, char **argv)
{
	SigTrans *InList, *stp, *Groups;
	FILE *fin;
	int count;
	int inlist_idx;
	int search_idx;
	int group_idx;
	int family_idx;
		
	if(argc == 2) {
		fin = fopen(argv[1],"rb");
	} else {
		fin = fopen("blocks/Tocta-9-31.blk","rb");
	}
	if(fin == NULL) {
		printf("Unable to open %s ... stopping.\n", argv[1]);
		exit(1);
	}
	
	count = 0;
	InList = (SigTrans*)malloc(sizeof(SigTrans));
	do {		
		fread(&(InList->transpose), sizeof(Matrix), 1, fin);
		if(feof(fin)) {
			free(InList);
		} else {
		++count;
		}
	} while(!(feof(fin)));
		
	const int len_inlist = count;
	printf("Length of inlist = %u\t", len_inlist);
	printf("Length mod 48 = %u\t", len_inlist % REFROTS);
	const int nGroups = len_inlist / REFROTS;
	printf("nGroups = %u\n", nGroups);
	
	// Groups will be indexed as [(group_idx * REFROTS) + family_idx]
	Groups = (SigTrans*)malloc(sizeof(SigTrans) * len_inlist);
		if(Groups == NULL) {
		printf("Failed to allocate for groups.\n");
		exit(1);
	}
	
	InList = (SigTrans*)malloc(sizeof(SigTrans) * len_inlist);
	if(InList == NULL) {
		printf("Failed to allocate for inlist.\n");
		exit(1);
	}
	
	rewind(fin);
	
	for(inlist_idx = 0; inlist_idx < len_inlist; ++inlist_idx) {
		stp = InList + inlist_idx;
		fread( &((stp)->transpose), sizeof(Matrix), 1, fin);
		stp->found = false;
	}
	// Note - Not setting Signatures since they are not referenced
	fclose(fin);


	
	// Groups treated as array[nGroups][REFROTS] of SigTrans structures
	
	// Set up a workarea SigTrans
	SigTrans *working = (SigTrans*)malloc(sizeof(SigTrans));	
	
	group_idx = 0;
	do {
		for(inlist_idx = 0; inlist_idx < len_inlist; ++inlist_idx) {
			if( (InList + inlist_idx)->found == false) 	break;
		}
		if(inlist_idx < len_inlist) {		
			stp = InList + inlist_idx;		
			// make a working copy of the base Tocta
			memcpy(working, stp, sizeof(SigTrans));
			family_idx = 0;
			printf("group_idx = %u\n",group_idx);
			// for each rotation
			for(int abcd = 0; abcd < 2; ++abcd) {
				for(int bd = 0; bd < 4; ++bd) {
					for(int ef = 0; ef < 4; ++ef) {
						//==================================================
						// find equal transpose - returns index to InList
						for(search_idx = 0; search_idx < len_inlist; ++search_idx)
							if(((InList + search_idx)->found == false)&&
								(equal_transpose(InList + search_idx, working) == 0)) break;
						if(search_idx == len_inlist) {
							printf("Failed to match SigTrans in InList.\n");
							exit(1);
						}			
						// set found flag in inlist to 'true'
						(InList + search_idx)->found = true;
										
						// memcpy(dest, source, bytes)
						
						memcpy((Groups + (group_idx*REFROTS) + family_idx++), (InList + search_idx), sizeof(SigTrans));
						//==================================================
						// apply_ef rotation
						working = gt_apply_ef(working);				
						
					} // for ef...
					working = gt_apply_bd(working);
					if(working == NULL) {
						printf("bd returned NULL.\n");
						exit(1);
					} 			
				} // for bd...
				
				working = gt_apply_ac(working);
				
				for(int ef = 0; ef < 4; ++ef) {					
					//==================================================
					// find equal transpose - returns index to InList
					for(search_idx = 0; search_idx < len_inlist; ++search_idx)
						if(((InList + search_idx)->found == false)&&
							(equal_transpose(InList + search_idx, working) == 0)) break;
					if(search_idx == len_inlist) {
						printf("Failed to match SigTrans in InList.\n");
						exit(1);
					}			
					// set found flag in inlist to 'true'
					(InList + search_idx)->found = true;
									
					// memcpy(dest, source, bytes)
					
					memcpy((Groups + (group_idx*REFROTS) + family_idx++), (InList + search_idx), sizeof(SigTrans));
					//==================================================
			
					// apply_ef rotation
					working = gt_apply_ef(working);				
															
				} // for ef...
				
				working = gt_apply_ac(working);
				working = gt_apply_ac(working);
				
				for(int ef = 0; ef < 4; ++ef) {					
					//==================================================
					// find equal transpose - returns index to InList
					for(search_idx = 0; search_idx < len_inlist; ++search_idx)
						if(((InList + search_idx)->found == false)&&
							(equal_transpose(InList + search_idx, working) == 0)) break;
					if(search_idx == len_inlist) {
						printf("Failed to match SigTrans in InList.\n");
						exit(1);
					}			
					// set found flag in inlist to 'true'
					(InList + search_idx)->found = true;
									
					// memcpy(dest, source, bytes)
					
					memcpy((Groups + (group_idx*REFROTS) + family_idx++), (InList + search_idx), sizeof(SigTrans));
					//==================================================
					
					// apply_ef rotation
					working = gt_apply_ef(working);				
													
				} // for ef...
				
				// Restore initial config - have 24 configs
				working = gt_apply_ac(working);
				
				// reflect
				working = gt_apply_abcd(working);
			
			} // for abcd...	

		}		
		group_idx++;
	} while(group_idx < nGroups);
	
	// output the Groups array
	for(int row = 0; row < nGroups; ++row) {
		for(int col = 0; col < REFROTS; ++col) {
			prt_sigtrans( Groups + (row * REFROTS) + col, col);
		}
	}
			
	// Free memory
	free(InList);
	free(Groups);
	free(working);
}

