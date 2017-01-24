/*
 * analysis.c
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
 * A data block is cut&pasted from the output of ps1
 * Code trys to find Tocta configs in data block
 * 
 */

#include "ps1.h"

	// permutations are for lists (order matters)
	int perms[][4] =
	{
		{0,1,2,3},{0,1,3,2},{0,2,1,3},{0,2,3,1},
		{0,3,1,2},{0,3,2,1},{1,0,2,3},{1,0,3,2},
		{1,2,0,3},{1,2,3,0},{1,3,0,2},{1,3,2,0},
		{2,0,1,3},{2,0,3,1},{2,1,0,3},{2,1,3,0},
		{2,3,0,1},{2,3,1,0},{3,0,1,2},{3,0,2,1},
		{3,1,0,2},{3,1,2,0},{3,2,0,1},{3,2,1,0}
	};

#if(1)


	gprime equalsums[][4] = // Sum = (7.0, 29.0)
	{ 
		{CMPLX(1.0,14.0),CMPLX(2.0,3.0),CMPLX(2.0,5.0),CMPLX(2.0,7.0)},
		{CMPLX(1.0,6.0),CMPLX(2.0,3.0),CMPLX(2.0,7.0),CMPLX(2.0,13.0)},
		{CMPLX(1.0,6.0),CMPLX(1.0,10.0),CMPLX(2.0,5.0),CMPLX(3.0,8.0)},
		{CMPLX(1.0,6.0),CMPLX(1.0,10.0),CMPLX(2.0,3.0),CMPLX(3.0,10.0)},
		{CMPLX(1.0,4.0),CMPLX(2.0,5.0),CMPLX(2.0,7.0),CMPLX(2.0,13.0)},
		{CMPLX(1.0,4.0),CMPLX(1.0,14.0),CMPLX(2.0,3.0),CMPLX(3.0,8.0)},
		{CMPLX(1.0,4.0),CMPLX(1.0,10.0),CMPLX(2.0,7.0),CMPLX(3.0,8.0)},
		{CMPLX(1.0,4.0),CMPLX(1.0,10.0),CMPLX(2.0,5.0),CMPLX(3.0,10.0)},
		{CMPLX(1.0,4.0),CMPLX(1.0,6.0),CMPLX(1.0,14.0),CMPLX(4.0,5.0)},
		{CMPLX(1.0,4.0),CMPLX(1.0,6.0),CMPLX(1.0,10.0),CMPLX(4.0,9.0)},
		{CMPLX(1.0,2.0),CMPLX(1.0,14.0),CMPLX(2.0,5.0),CMPLX(3.0,8.0)},
		{CMPLX(1.0,2.0),CMPLX(1.0,14.0),CMPLX(2.0,3.0),CMPLX(3.0,10.0)},
		{CMPLX(1.0,2.0),CMPLX(1.0,10.0),CMPLX(2.0,7.0),CMPLX(3.0,10.0)},
		{CMPLX(1.0,2.0),CMPLX(1.0,6.0),CMPLX(2.0,13.0),CMPLX(3.0,8.0)},
		{CMPLX(1.0,2.0),CMPLX(1.0,6.0),CMPLX(1.0,10.0),CMPLX(4.0,11.0)},
		{CMPLX(1.0,2.0),CMPLX(1.0,4.0),CMPLX(2.0,13.0),CMPLX(3.0,10.0)},
		{CMPLX(1.0,2.0),CMPLX(1.0,4.0),CMPLX(1.0,14.0),CMPLX(4.0,9.0)}
	};

#else

	gprime equalsums[][4] =
	{ // equalsums (8 + 26i)
		{CMPLX(1.0,10.0),CMPLX(2.0,3.0),CMPLX(2.0,5.0),CMPLX(3.0,8.0)},
		{CMPLX(1.0,6.0),CMPLX(2.0,5.0),CMPLX(2.0,7.0),CMPLX(3.0,8.0)},
		{CMPLX(1.0,6.0),CMPLX(2.0,3.0),CMPLX(2.0,7.0),CMPLX(3.0,10.0)},
		{CMPLX(1.0,6.0),CMPLX(1.0,10.0),CMPLX(2.0,5.0),CMPLX(4.0,5.0)},
		{CMPLX(1.0,4.0),CMPLX(2.0,5.0),CMPLX(2.0,7.0),CMPLX(3.0,10.0)},
		{CMPLX(1.0,4.0),CMPLX(1.0,14.0),CMPLX(2.0,3.0),CMPLX(4.0,5.0)},
		{CMPLX(1.0,4.0),CMPLX(1.0,10.0),CMPLX(2.0,7.0),CMPLX(4.0,5.0)},
		{CMPLX(1.0,4.0),CMPLX(1.0,10.0),CMPLX(2.0,3.0),CMPLX(4.0,9.0)},
		{CMPLX(1.0,4.0),CMPLX(1.0,6.0),CMPLX(2.0,7.0),CMPLX(4.0,9.0)},
		{CMPLX(1.0,4.0),CMPLX(1.0,6.0),CMPLX(2.0,5.0),CMPLX(4.0,11.0)},
		{CMPLX(1.0,4.0),CMPLX(1.0,6.0),CMPLX(1.0,10.0),CMPLX(5.0,6.0)},
		{CMPLX(1.0,2.0),CMPLX(2.0,3.0),CMPLX(2.0,13.0),CMPLX(3.0,8.0)},
		{CMPLX(1.0,2.0),CMPLX(1.0,14.0),CMPLX(2.0,5.0),CMPLX(4.0,5.0)},
		{CMPLX(1.0,2.0),CMPLX(1.0,10.0),CMPLX(2.0,5.0),CMPLX(4.0,9.0)},
		{CMPLX(1.0,2.0),CMPLX(1.0,10.0),CMPLX(2.0,3.0),CMPLX(4.0,11.0)},
		{CMPLX(1.0,2.0),CMPLX(1.0,6.0),CMPLX(3.0,8.0),CMPLX(3.0,10.0)},
		{CMPLX(1.0,2.0),CMPLX(1.0,6.0),CMPLX(2.0,13.0),CMPLX(4.0,5.0)},
		{CMPLX(1.0,2.0),CMPLX(1.0,6.0),CMPLX(2.0,7.0),CMPLX(4.0,11.0)},
		{CMPLX(1.0,2.0),CMPLX(1.0,6.0),CMPLX(1.0,10.0),CMPLX(5.0,8.0)},
		{CMPLX(1.0,2.0),CMPLX(1.0,4.0),CMPLX(1.0,14.0),CMPLX(5.0,6.0)}
	};

#endif

// -----Main-----

int main(int argc, char **argv)
{
	// declare indexes for selecting rows
	int a,b,c,d,candidates;
	// declare a matrix for the working config
	
	gprime working_cfg[4][4];
	
	// declare pair data blocks
	// PairData pdr0r1,pdr0r2,pdr1r2,pdr0r3,pdr1r3,pdr2r3;
	
	// List of PairDataV3 structures
	GSList *PairsV3 = NULL;
	PairDataV3 *pdrarb = NULL;
	
	// declare return value
	int rv;
	
	// Determine the number of rows in the equalsums matrix
	const int nRows = sizeof(equalsums)/sizeof(gprime)/4;	
	
	// select 4 rows from equal sums
	candidates = 0;
	for(a = 0; a < nRows; ++a) {
		for(b = 0; b < nRows; ++b) {
			if (a == b) continue;
			// load first two rows and test for validity
			for(int x = 0; x < 4; ++x) {
				working_cfg[0][x] = equalsums[a][x];
				working_cfg[1][x] = equalsums[b][x];
			}
			
			// count pairs in row0 and row1
			pdrarb = (PairDataV3*)malloc(sizeof(PairDataV3));
			if(count_pairs_V3(pdrarb, (gprime*)working_cfg, 0, 1) != 1) {
				free(pdrarb);
				continue;
			} else {
				PairsV3 = g_slist_append(PairsV3, pdrarb);
			}
			
			// Add another row to the working config
			for(c = 0; c < nRows; ++c) {
				// load the third row
				for(int x = 0; x < 4; ++x)
					working_cfg[2][x] = equalsums[c][x];
				// test for validity
				pdrarb = (PairDataV3*)malloc(sizeof(PairDataV3));
				if(count_pairs_V3(pdrarb, (gprime*)working_cfg, 0, 2) != 0) {
					free(pdrarb);
					continue;
				}
				// Here the PairDataV3 structure remains valid
				if(count_pairs_V3(pdrarb, (gprime*)working_cfg, 1, 2) != 1) {
					free(pdrarb);
					continue;
				}
				PairsV3 = g_slist_append(PairsV3, pdrarb);

				// Add final row
				for(d = 0; d < nRows; ++d) {
					// load the fourth row
					for(int x = 0; x < 4; ++x)
						working_cfg[3][x] = equalsums[d][x];
					// test for validity
					pdrarb = (PairDataV3*)malloc(sizeof(PairDataV3));
					if(count_pairs_V3(pdrarb , (gprime*)working_cfg, 1, 3) != 0) {
						free(pdrarb);
						continue;
					}
					// Here the PairDataV3 structure remains valid
					if(count_pairs_V3(pdrarb, (gprime*)working_cfg, 0, 3) != 1) {
						free(pdrarb);
						continue;
					} else {
						PairsV3 = g_slist_append(PairsV3, pdrarb);
					}
					pdrarb = (PairDataV3*)malloc(sizeof(PairDataV3));
					if(count_pairs_V3(pdrarb, (gprime*)working_cfg, 2, 3) != 1) {
						free(pdrarb);
						continue;
					} else {
						PairsV3 = g_slist_append(PairsV3, pdrarb);
					}
					// if we get here then the working config may be a tocta candidate
					printf("-----Candidate-----\n");
					prt_working_cfg((gprime*)working_cfg,4);
					printf("\n");
					++candidates;
					
					// allocate a transpose matrix
					// from working_cfg, PairsV3 list and transpose matrix
					// 		copy pairs to transpose and set sources to zero
					//		copy remaining row values
					// permute col2/col3 values and look for column sums == target 
					
					
				} // for d...
			} // for c...
		} // for b ...
	} // for a ...
	
	printf("Number of candidates found: %d  ",candidates);
	printf("Rows scanned: %d\n",nRows);
	return 0;
}

