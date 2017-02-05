/*
 * utilities.c
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

// function definitions

int comparedouble (const void * a, const void * b)
{
  return ( *(double*)a - *(double*)b );
}

gint compare_fps(gconstpointer a, gconstpointer b) {
	FourPrimeSum a_fps, b_fps;
	a_fps = (*(FourPrimeSum*)a);
	b_fps = (*(FourPrimeSum*)b);
	double complex *a_total, *b_total;
	a_total = &(a_fps.total);
	b_total = &(b_fps.total);
	return compare_gprime((gconstpointer)a_total, (gconstpointer)b_total);
}

gint compare_gprime(gconstpointer a, gconstpointer b)
{
	double a_real,b_real,a_imag,b_imag;
	
	a_real = creal(*(gprime*)a);
	b_real = creal(*(gprime*)b);
	if(a_real < b_real) return -1;
	if(a_real > b_real) return +1;
	// test imag components
	a_imag = cimag(*(gprime*)a);
	b_imag = cimag(*(gprime*)b);
	if(a_imag < b_imag) return -1;
	if(a_imag > b_imag) return +1;
	// Equal return 0;
	return 0;
}


int count_pairs(gprime *p, gprime*q) 
{
	// Assumes rows of 4 values
	int a,b,pair_count;
	pair_count = 0;
	for(a = 0; a < 4; ++a) {
		for(b = 0; b < 4; ++b) {
			if(compare_gprime((p+a),(q+b)) == 0) pair_count += 1;
		}
	}
	return pair_count;
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

int count_pairs_V3(PairDataV3 *pd3, gprime *working_cfg, int rowa, int rowb)
{
	// setup pointers to row start
	gprime *a = working_cfg + (rowa * 4);
	gprime *b = working_cfg + (rowb * 4);
	// initialise the data area
	pd3->count = 0;
	pd3->matchA = NULL;
	pd3->matchB = NULL;
	pd3->value = CMPLX(0.0,0.0);
	for(int cola = 0; cola < 4; ++cola) {
		for(int colb = 0; colb < 4; ++colb) {
			if(compare_gprime((a + cola),(b + colb)) == 0) {
				++pd3->count;
				pd3->matchA = a + cola;
				pd3->matchB = b + colb;
				pd3->value = *(pd3->matchA);
			}
			if(pd3->count > 1) {
				pd3->matchA = NULL;
				pd3->matchB = NULL;
				pd3->value = CMPLX(0.0,0.0);
				return(pd3->count);
			}
		} // for colb...
	} // for cola...
	return(pd3->count);
}

void prt_working_cfg(gprime *cfg, int rows) {
	if((rows<1)||(rows>4)) {
		printf("prt_working_cfg row error: %d\n", rows);
		return;
	}
	for(int r = 0; r < rows; ++r) {
		for(int c = 0; c < 4; ++c) {
			//printf("%.0f + %.0fi\t", creal(*(cfg + (r*4) + c)), cimag(*(cfg + (r*4) + c)));
			prt_gprime(*(cfg + (r*4) + c));
		}
		printf("\n");
	}
}

int transpose_wcfg(gprime *wcfg, PairData *pdat, int idx_a_dest, int idx_b_dest)
{
	int a_src  = (pdat->rowa*4) + (pdat->idxa);
	int a_dest = (pdat->rowa*4) + idx_a_dest;
	int b_src  = (pdat->rowb*4) + (pdat->idxb);
	int b_dest = (pdat->rowb*4) + idx_b_dest;
	
	gprime a = *(wcfg + a_src);
	gprime b = *(wcfg + b_src);
	// Sanity check for PairData
	if (a != b) {
		printf("\nUnequal pair value found.\n");
		return(1);
	}
	if (idx_a_dest == idx_b_dest) return(2);
	if ((wcfg == NULL)||(pdat == NULL)) return(3);
	
	// Note - if src and dest offsets are equal then value is already in place!
	
	// transpose row a
	if(a_src != a_dest) {
		// mov src to temp - already done! - gprime a has value
		// mov dest to src
		*(wcfg + a_src) = *(wcfg + a_dest);	
		// mov temp to dest
		*(wcfg + a_dest) = a;
	}
	
	// transpose row b
	if(b_src != b_dest) {
		// mov src to temp - already done! - gprime b has value
		// mov dest to src
		*(wcfg + b_src) = *(wcfg + b_dest);	
		// mov temp to dest
		*(wcfg + b_dest) = b;
	}
		
	return(0);
}


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

void prt_sigtrans(SigTrans *stp, int idx) {
	for(int row = 0; row < 4; ++row) {
		if(row == 0) {
			printf("%02d)\t", idx);
		} else {
			printf("\t");
		}
		for(int col = 0; col < 4; ++col) {			
			prt_gprime(stp->transpose[row][col]);
		}
		printf("\t");
		for(int col = 0; col < 3; ++col) {
			prt_gprime(stp->signature[row*3 + col]);
		}
		printf("\n");
	}
	printf("\n");	
}



// ----------------------------------------------------------------------
