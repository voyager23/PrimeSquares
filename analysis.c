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

#if(0)


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
	
	// from typedef gprime Matrix[4][4];
	Matrix working_cfg;
	Matrix transpose;
	
	PairData pdarray[4];	// use an array of PairData
	
	gprime Target = equalsums[0][0] + equalsums[0][1] + equalsums[0][2] + equalsums[0][3];
	
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
			if(count_pairs_V2(pdarray + 0, (gprime*)working_cfg, 0, 1) != 1) continue;			
			
			// Add another row to the working config
			for(c = 0; c < nRows; ++c) {
				// load the third row
				for(int x = 0; x < 4; ++x)
					working_cfg[2][x] = equalsums[c][x];
				// test for validity
				if(count_pairs_V2(pdarray + 1, (gprime*)working_cfg, 0, 2) != 0) continue;
				// Here the PairData structure remains valid
				if(count_pairs_V2(pdarray + 1, (gprime*)working_cfg, 1, 2) != 1) continue;
				// Add final row
				for(d = 0; d < nRows; ++d) {
					// load the fourth row
					for(int x = 0; x < 4; ++x)
						working_cfg[3][x] = equalsums[d][x];
					// test for validity
					if(count_pairs_V2(pdarray + 2 , (gprime*)working_cfg, 1, 3) != 0) continue;
					
					// Here the PairData structure remains valid
					if(count_pairs_V2(pdarray + 2, (gprime*)working_cfg, 0, 3) != 1) continue;
					
					if(count_pairs_V2(pdarray + 3, (gprime*)working_cfg, 2, 3) != 1) continue;

					// if we get here then the working config may be a tocta candidate
					printf("-----Candidate-----\n");
					prt_working_cfg((gprime*)working_cfg,4);
					printf("\n");
					++candidates;
										
					// using transpose matrix, PairData array and working_cfg ...
					for(int x = 0; x < 4; ++x) {
						// set the value in the transpose matrix
						transpose[ pdarray[x].rowa ][ pdarray[x].tara ] = pdarray[x].value;
						transpose[ pdarray[x].rowb ][ pdarray[x].tarb ] = pdarray[x].value;
						// tag the value in the working_cfg with *=-1
						working_cfg[ pdarray[x].rowa ][ pdarray[x].idxa ] *= -1;
						working_cfg[ pdarray[x].rowb ][ pdarray[x].idxb ] *= -1;												
					}
					
					printf("-----Candidate post initial transpose\n");
					prt_working_cfg((gprime*)working_cfg,4);
					printf("\n");
					
					// now fill in unused values again tagging used values
					for(int row = 0; row < 4; ++row) {
						for(int target = 2; target < 4; ++target) {
							for(int col = 0; col < 4; ++col) {
								if( creal(working_cfg[row][col]) >= 0.0 ) {
									transpose[row][target] = working_cfg[row][col];	// copy unused value
									working_cfg[row][col] *= -1;					// tag as used
									break;											// next target column
								}
							} // for col...
						} // for target...
					} // for row...
					
					// reset the working_cfg
					for(int row=0;row<4;++row)
						for(int col=0;col<4;++col) 
							working_cfg[row][col] *= -1;
					
					printf("-----Final Candidate-----\n");
					prt_working_cfg((gprime*)working_cfg,4);
					printf("\n");					
					
					printf("-----Final Transpose-----\n");
					prt_working_cfg((gprime*)transpose,4);
					printf("\n");
	
					printf("======================================\n");

					// permute col2/col3 values and look for column sums == target
					// if solution found add transpose to GSList Toctas else free transpose matrix
					
					gprime swap;
					for(int row0 = 0; row0 < 2; ++row0) {
						for(int row1 = 0; row1 < 2; ++row1) {
							for(int row2 = 0; row2 < 2; ++row2) {
								for(int row3 = 0; row3 < 2; ++row3) {
									// test config
									printf("-----Swap Transpose-----\n");
									prt_working_cfg((gprime*)transpose,4);
									printf("Target: ");
									prt_gprime(Target);
									printf("\n");
									// record any success
									gprime colsum2 = transpose[0][2] + transpose[1][2] + transpose[2][2] + transpose[3][2]; 
									gprime colsum3 = transpose[0][3] + transpose[1][3] + transpose[2][3] + transpose[3][3];
									// printf("Colsum2 = "); prt_gprime(colsum2); printf("\n"); 
									// printf("Colsum3 = "); prt_gprime(colsum3); printf("\n");
									if((Target == colsum2)&&(Target == colsum3)) { 
										printf("TOCTA!\n");
										exit(0);
									}
									// swap values in row 3 cols 2 & 3
									swap = transpose[3][2]; transpose[3][2] = transpose[3][3]; transpose[3][3] = swap;
								}
								// swap values in row 2 cols 2 & 3
								swap = transpose[2][2]; transpose[2][2] = transpose[2][3]; transpose[2][3] = swap;
							}
							// swap values in row 1 cols 2 & 3
							swap = transpose[1][2]; transpose[1][2] = transpose[1][3]; transpose[1][3] = swap;

						}
						// swap values in row 0 cols 2 & 3
						swap = transpose[0][2]; transpose[0][2] = transpose[0][3]; transpose[0][3] = swap;
					}
				} // for d...
			} // for c...
		} // for b ...
	} // for a ...
	
	printf("Number of candidates found: %d  ",candidates);
	printf("Rows scanned: %d\n",nRows);
	return 0;
}

