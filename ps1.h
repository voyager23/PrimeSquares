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

	typedef double complex gprime;

	typedef struct fps {
		complex double total;
		complex double p0,p1,p2,p3;
	} FourPrimeSum;

	// Convenience Macros
	#define nth_gprime(head,n) *((gprime*)g_slist_nth_data(head,n))
	#define prt_gprime(a) printf("(%.1f,%.1f)", creal(a), cimag(a))
	
	
#endif


