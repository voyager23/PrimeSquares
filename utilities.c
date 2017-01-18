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
	pd->idxa = -1;
	pd->idxb = -1;
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

