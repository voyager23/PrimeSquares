/*
 * ps1.h
 * 
 * Copyright 2016 mike <mike@localhost.localdomain>
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

#ifndef __PS1_H__
#define __PS1_H__

	#include <stdio.h>
	#include <stdlib.h>
	#include <complex.h>
	#include <locale.h>
	#include <glib.h>

	typedef double complex gprime;	// gprime is 16 bytes
	
	typedef gprime Matrix[4][4];

	typedef struct fps {
		gprime total;
		gprime p0,p1,p2,p3;
	} FourPrimeSum;
	
	typedef struct pd {
		int count;		// pairs found
		int rowa,rowb;	// row numbers (0-3)
		int idxa, idxb;	// indexes of -last- pair found
		int tara, tarb;	// target colums for this pair
		gprime value;	// -last- value found
	} PairData;
	
	typedef struct pd3 {
		int count;		// 0 <= pairs found <= 1 
		gprime *matchA, *matchB;	// unique row pair in working_cfg
		gprime value;	// last value found
	}PairDataV3;
	
	// function declarations
	int count_pairs(gprime *p, gprime*q);
	gint compare_gprime(gconstpointer a, gconstpointer b);
	void prt_working_cfg(gprime *cfg, int rows);
	int count_pairs_V2(PairData *pd, gprime *working_cfg, int rowa, int rowb);
	int count_pairs_V3(PairDataV3 *pd3, gprime *working_cfg, int rowa, int rowb);
	int transpose_wcfg(gprime *wcfg, PairData *pdat, int row_a_dest, int row_b_dest);

	
	// Convenience Macros
	#define nth_gprime(head,n) *((gprime*)g_slist_nth_data(head,n))
	#define prt_gprime(a) printf("(%d,%di)  ", (int)creal(a), (int)cimag(a))
	
#endif


