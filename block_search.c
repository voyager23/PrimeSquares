/*
 * block_search.c
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


#include "ps1.h"
// Sum to (14.0,24.0)
const double complex array1[][4] =
{
CMPLX(2.0,5.0), CMPLX(3.0,8.0), CMPLX(4.0,5.0), CMPLX(5.0,6.0),
CMPLX(2.0,3.0), CMPLX(3.0,8.0), CMPLX(4.0,5.0), CMPLX(5.0,8.0),
CMPLX(2.0,3.0), CMPLX(2.0,7.0), CMPLX(5.0,6.0), CMPLX(5.0,8.0),
CMPLX(2.0,3.0), CMPLX(2.0,5.0), CMPLX(3.0,8.0), CMPLX(7.0,8.0),
CMPLX(1.0,6.0), CMPLX(2.0,5.0), CMPLX(4.0,5.0), CMPLX(7.0,8.0),
CMPLX(1.0,4.0), CMPLX(4.0,5.0), CMPLX(4.0,9.0), CMPLX(5.0,6.0),
CMPLX(1.0,4.0), CMPLX(2.0,7.0), CMPLX(4.0,5.0), CMPLX(7.0,8.0),
CMPLX(1.0,4.0), CMPLX(2.0,3.0), CMPLX(4.0,9.0), CMPLX(7.0,8.0),
CMPLX(1.0,4.0), CMPLX(1.0,6.0), CMPLX(5.0,6.0), CMPLX(7.0,8.0),
CMPLX(1.0,2.0), CMPLX(4.0,5.0), CMPLX(4.0,9.0), CMPLX(5.0,8.0),
CMPLX(1.0,2.0), CMPLX(3.0,8.0), CMPLX(5.0,6.0), CMPLX(5.0,8.0),
CMPLX(1.0,2.0), CMPLX(2.0,5.0), CMPLX(4.0,9.0), CMPLX(7.0,8.0),
CMPLX(1.0,2.0), CMPLX(1.0,6.0), CMPLX(5.0,8.0), CMPLX(7.0,8.0)
};

// Sum to (14,26.0)
const double complex array2[][4] =
{
CMPLX(2.0,7.0), CMPLX(3.0,8.0), CMPLX(4.0,5.0), CMPLX(5.0,6.0),
CMPLX(2.0,5.0), CMPLX(3.0,8.0), CMPLX(4.0,5.0), CMPLX(5.0,8.0),
CMPLX(2.0,5.0), CMPLX(2.0,7.0), CMPLX(5.0,6.0), CMPLX(5.0,8.0),
CMPLX(2.0,3.0), CMPLX(3.0,8.0), CMPLX(4.0,9.0), CMPLX(5.0,6.0),
CMPLX(2.0,3.0), CMPLX(2.0,7.0), CMPLX(3.0,8.0), CMPLX(7.0,8.0),
CMPLX(1.0,6.0), CMPLX(4.0,5.0), CMPLX(4.0,9.0), CMPLX(5.0,6.0),
CMPLX(1.0,6.0), CMPLX(2.0,7.0), CMPLX(4.0,5.0), CMPLX(7.0,8.0),
CMPLX(1.0,6.0), CMPLX(2.0,3.0), CMPLX(4.0,9.0), CMPLX(7.0,8.0),
CMPLX(1.0,4.0), CMPLX(4.0,5.0), CMPLX(4.0,9.0), CMPLX(5.0,8.0),
CMPLX(1.0,4.0), CMPLX(3.0,8.0), CMPLX(5.0,6.0), CMPLX(5.0,8.0),
CMPLX(1.0,4.0), CMPLX(2.0,5.0), CMPLX(4.0,9.0), CMPLX(7.0,8.0),
CMPLX(1.0,4.0), CMPLX(1.0,6.0), CMPLX(5.0,8.0), CMPLX(7.0,8.0),
CMPLX(1.0,2.0), CMPLX(2.0,7.0), CMPLX(4.0,9.0), CMPLX(7.0,8.0)
};

int main(int argc, char ** argv) {
	const gprime * a;
	const gprime * b;
	const gprime * c;
	const gprime * d;
	const gprime * sentinel = &array1[12][3];
	
	gprime pairs[8];
	gprime *p, *q;
	
	int count = 0;
	for(a = &array1[0][0]; a < sentinel; a += 4) {
		for(b = a + 4; b < sentinel; b += 4) {
			prt_gprime(*a); printf("\t");
			prt_gprime(*b); printf("\n");
			++count;
			p = pairs;
			q = p+4;
			for(int x=0;x<4;x++) { *(p+x) = *(a+x); *(q+x) = *(b+x); }
			int dups = 0;
			for(p = pairs; p < &pairs[8]; p++) {
				for(q = p+1; q < &pairs[8]; q++) {
					if((*p)==(*q)) {
						++dups;
						printf("Dups: "); prt_gprime(*p); printf(" => "); prt_gprime(*q); printf("\n");
					}
				}
			}
			printf("Dups: %i\n",dups);
		}
		printf("\n");
	}
	printf("Count: %i\n",count);
	return 0;
}

