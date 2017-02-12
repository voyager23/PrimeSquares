/*
 * lib_sym.c
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
//----------------------------------------------------------------------

SigTrans *init_sigtrans(SigTrans *data) {
	if(data != NULL) {
		// ------------------start init---------------------------------
		for(int i = 0; i < 16; ++i) {
			data->transpose[i / 4][i % 4] = CMPLX((double)i,(double)i);
		}
		for(int i = 0, s = 11; i < 12; ++i,--s) {
			data->sig_major[i] = data->transpose[s/3][(s%3)+1];
			data->sig_minor[i] = CMPLX(0.0,0.0);
		}
		// correct column 0 to show matching values
		for(int r = 3; r > -1; --r) 
			data->transpose[(r+1)%4][0] = data->transpose[r][1];
		// =INIT SPECIFIC= put some values in sig_minor
		for(int r = 0; r < 4; ++r) data->sig_minor[r] = data->sig_major[r*3 + 2];

		// ------------------end init-----------------------------------		
	}
	return data;
}

//----------------------------------------------------------------------
SigTrans* gt_apply_abcd(SigTrans *st) {
	// Complete rewrite of reflection about horizontal
	// plane abcd.
	// Allocate a new SigTrans;
	// Copy sig_major;
	// Copy gprimes to locations in new transpose as required
	// Update and sort sig_minor for new config
	// free(old pointer);
	// return new pointer
	
	SigTrans *data = (SigTrans*)malloc(sizeof(SigTrans));
	// copy sig_major, zeroise sig_minor;
	for(int x = 0; x < 12; ++x) {
		data->sig_major[x] = st->sig_major[x];
		data->sig_minor[x] = CMPLX(0.0,0.0);
	}
	// copy gprimes to new locations as required
	// The swap proceeds row by row
	for(int row = 0; row < 4; ++row) {
		// copy cols 0 and 1 in order
		data->transpose[row][0] = st->transpose[row][0];
		data->transpose[row][1] = st->transpose[row][1];
		data->transpose[row][2] = st->transpose[row][3];	//swap
		data->transpose[row][3] = st->transpose[row][2];	//swap
	}
		
	// Update sig_minor
	for(int x = 0; x < 4; ++x) data->sig_minor[x] = st->transpose[x][1];
	
	// qsort minor signature (msb first)
	qsort((void*)data->sig_minor, 12, sizeof(gprime), qsort_signature_compare);
	
	// free old pointer and return new value
	free(st);
	
	return(data);
}
		
		
//----------------------------------------------------------------------
void gt_apply_ef(SigTrans *st) {
	gprime gprimes[16];
	gprime *st_prime = (gprime*)(&(st->transpose));

}

//----------------------------------------------------------------------
void gt_apply_bd(SigTrans *st) {
	gprime gprimes[16];
	gprime *st_prime = (gprime*)(&(st->transpose));

}

//----------------------------------------------------------------------
void gt_apply_ac(SigTrans *st){
	gprime gprimes[16];
	gprime *st_prime = (gprime*)(&(st->transpose));

}

//----------------------------------------------------------------------
