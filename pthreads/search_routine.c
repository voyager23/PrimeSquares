/*
 * search_routine.c
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

void * search_routine(void *tdp) {
		
	int my_idx = ((ThreadData*)tdp)->idx;	
	printf("\tIn search_routine %i.\n", my_idx);
	// code for tocta search
	Matrix working_cfg;
	Matrix transpose;
	PairData pdarray[4];	// use an array of PairData
	int nRows = ((ThreadData*)tdp)->nRows;
	gpRow *equalsums = ((ThreadData*)tdp)->row_ptr;
	gprime Target = equalsums[0][0] + equalsums[0][1] + equalsums[0][2] + equalsums[0][3];
		
	// select 4 rows from equal sums array
	int candidates = 0;
	int toctas = 0;
	for(int a = 0; a < nRows; ++a) {
		for(int b = 0; b < nRows; ++b) {
			if (a == b) continue;
			// load first two rows and test for validity
			for(int x = 0; x < 4; ++x) {
				working_cfg[0][x] = equalsums[a][x];
				working_cfg[1][x] = equalsums[b][x];
			}
			
			// count pairs in row0 and row1
			if(count_pairs_V2(pdarray + 0, (gprime*)working_cfg, 0, 1) != 1) continue;			
			
			// Add another row to the working config
			for(int c = 0; c < nRows; ++c) {
				// load the third row
				for(int x = 0; x < 4; ++x)
					working_cfg[2][x] = equalsums[c][x];
				// test for validity
				if(count_pairs_V2(pdarray + 1, (gprime*)working_cfg, 0, 2) != 0) continue;
				// Here the PairData structure remains valid
				if(count_pairs_V2(pdarray + 1, (gprime*)working_cfg, 1, 2) != 1) continue;
				// Add final row
				for(int d = 0; d < nRows; ++d) {
					// load the fourth row
					for(int x = 0; x < 4; ++x)
						working_cfg[3][x] = equalsums[d][x];
					// test for validity
					if(count_pairs_V2(pdarray + 2 , (gprime*)working_cfg, 1, 3) != 0) continue;
					
					// Here the PairData structure remains valid
					if(count_pairs_V2(pdarray + 2, (gprime*)working_cfg, 0, 3) != 1) continue;
					
					if(count_pairs_V2(pdarray + 3, (gprime*)working_cfg, 2, 3) != 1) continue;

					// if we get here then the working config may be a tocta candidate
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
					
					// permute col2/col3 values and look for 2 column sums == target
					gprime swap;
					for(int row0 = 0; row0 < 2; ++row0) {
						for(int row1 = 0; row1 < 2; ++row1) {
							for(int row2 = 0; row2 < 2; ++row2) {
								for(int row3 = 0; row3 < 2; ++row3) {
									// test config
									gprime colsum2 = transpose[0][2] + transpose[1][2] + transpose[2][2] + transpose[3][2]; 
									gprime colsum3 = transpose[0][3] + transpose[1][3] + transpose[2][3] + transpose[3][3];
									if((Target == colsum2)&&(Target == colsum3)) {
										// --------------------------------------------------------------------
										// At this point the transpose matrix holds a valid Tocta configuration
										// --------------------------------------------------------------------
										++toctas;
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
	printf("EqualSum rows scanned: %d\n", nRows);
	printf("Target: ");
	prt_gprime(Target);
	printf("\n");
	printf("Number of candidates found: %d\n", candidates);
	printf("Number of Tocta configurations found: %d\n", toctas);
	printf("Number of unique Toctas may be %d	(modcheck=%d)\n", toctas/48, toctas%48);
	free(equalsums);
		
	// search terminated
	pthread_exit(NULL);
}

int count_pairs_V2(PairData *pd, gprime *working_cfg, int rowa, int rowb)
{
	// setup pointers
	gprime *a = working_cfg + (rowa * 4);
	gprime *b = working_cfg + (rowb * 4);
	// initialise the data area
	pd->count = 0;
	// These are the source co-ords
	pd->rowa = rowa;
	pd->rowb = rowb;
	pd->idxa = -1;
	pd->idxb = -1;
	// These are the target co-ords
	if((rowb - rowa) == 1) {
		pd->tara = 1;
		pd->tarb = 0;
	} else {
	// adjust for final pair
		pd->tara = 0;
		pd->tarb = 1;
	}	
	// scan for pairs
	pd->value = CMPLX(0.0,0.0);	
	for(int cola = 0; cola < 4; ++cola) {
		for(int colb = 0; colb < 4; ++colb) {
			if(compare_gprime((a + cola),(b + colb)) == 0) {
				++pd->count;
				pd->idxa = cola;
				pd->idxb = colb;
				pd->value = *(a + cola);
			}
			if(pd->count > 1) return(pd->count);
		}
	}
	return(pd->count);
}

gint compare_gprime(gconstpointer a, gconstpointer b)
{
	double a_real,b_real,a_imag,b_imag;
	
	a_real = creal(*(double complex*)a);
	b_real = creal(*(double complex*)b);
	if(a_real < b_real) return -1;
	if(a_real > b_real) return +1;
	// test imag components
	a_imag = cimag(*(double complex*)a);
	b_imag = cimag(*(double complex*)b);
	if(a_imag < b_imag) return -1;
	if(a_imag > b_imag) return +1;
	// Equal return 0;
	return 0;
}
