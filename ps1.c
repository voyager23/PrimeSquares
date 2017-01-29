/*
 * ps1.c
 * 
 * Copyright 2016 mike <mike@mike-NC10-N110>
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
 * 	General search for numbers that are the sum of two squares p & q
 * 	Range: 0 <= p | q <= 30
 */


#include "ps1.h"
#include <glib-unix.h>

const double primes[] =
{ 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 
71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 
151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 
229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 
311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 
397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 
479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 
577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 
659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 
757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839, 853, 
857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947, 
953, 967, 971, 977, 983, 991, 997, 1009, 1013, 1019, 1021, 1031, 1033, 
1039, 1049, 1051, 1061, 1063, 1069, 1087, 1091, 1093, 1097, 1103, 1109, 
1117, 1123, 1129, 1151, 1153, 1163, 1171, 1181, 1187, 1193, 1201, 1213, 
1217, 1223, 1229, 1231, 1237, 1249, 1259, 1277, 1279, 1283, 1289, 1291, 
1297, 1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 1373, 1381, 1399, 
1409, 1423, 1427, 1429, 1433, 1439, 1447, 1451, 1453, 1459, 1471, 1481, 
1483, 1487, 1489, 1493, 1499, 1511, 1523, 1531, 1543, 1549, 1553, 1559, 
1567, 1571, 1579, 1583, 1597, 1601, 1607, 1609, 1613, 1619, 1621, 1627, 
1637, 1657, 1663, 1667, 1669, 1693, 1697, 1699, 1709, 1721, 1723, 1733, 
1741, 1747, 1753, 1759, 1777, 1783, 1787, 1789, 1801, 1811, 1823, 1831, 
1847, 1861, 1867, 1871, 1873, 1877, 1879, 1889, 1901, 1907, 1913, 1931, 
1933, 1949, 1951, 1973, 1979, 1987, 1993, 1997, 1999 };



//-----Main-----
int main(int argc, char **argv)
{
	double limit;
	if(argc == 2) {
		sscanf(argv[1], "%lf", &limit);
	} else {
		limit = 14.0;
	}
	
	printf("Limit: %0.0lf\n", limit);
	
	// exit(0);
		
	// limit 18 produces 48 blocks. Threaded scan (all cores) takes 3m 06s
	// const double limit=14.0;	// sqrt(1000) = 31.622	use 14 for 28 blocks of data
	double p,q,si,s;
	int found = 0;
	
	GSList *head = NULL;
	GSList *blk_start, *working, *write_ptr;
	gprime *gprime_ptr;
	
	// Working file
	char *outfile="equalsums.dat";
	
	setlocale(LC_ALL,"");
	
	printf("Using %2.0f primes from list.\n",limit);
	
	for(p=0; p<limit; p++) {
		si = p*p;
		for(q=p+1; q<limit; q++) {
			s = q*q + si;
			int * pItem;
			pItem = (int*) bsearch(&s, primes, (sizeof(primes)/sizeof(double)), sizeof(double), comparedouble);
			if (pItem!=NULL) {
			printf ("%.1f^2 + %.1f^2 \t= %4.0f  Sum of squares is normal prime.\n",p,q,s);
			found += 1;
			// glib singly linked list
			gprime_ptr = (gpointer)malloc(sizeof(gprime));
			*gprime_ptr = CMPLX(p,q);
			head = g_slist_prepend(head,(gpointer)gprime_ptr);			 
			}
		}
	}
	printf("Found %i results.\n",found);
	printf("4_index search space = %'d\n",(found*(found-1)*(found-2)*(found-3)));
	
	
	printf("Sorting list...");
	head = g_slist_sort(head,compare_gprime);
	printf("complete.\n");
	
	
	// Iterate over the g_slist of gprimes and print contents
	working = head;
	found = 0;
	while(working != NULL) {
		found += 1;
		gprime_ptr = (gprime*)working->data;
		printf ("%.1f^2 + %.1f^2   \tis in the g_slist.\n", creal(*gprime_ptr), cimag(*gprime_ptr));
		working = g_slist_next(working);
	}
	printf("Found %i items in l_list.\n",found);
	printf("g_slist_length() returned %i items.\n", g_slist_length(head));
	
	// Do 4 index search over the g_slist of gprimes
	int a,b,c,d;
	gprime s0, s1, s2, s3;
	int list_size = g_slist_length(head);
	GSList *Sums = NULL;
	FourPrimeSum *fps_ptr;
	
	for(a = 0; a < list_size; ++a) {
		s0 = nth_gprime(head,a);
		for(b = a + 1; b < list_size; ++b) {
			// we can add complex doubles using '+' operator
			s1 = s0 + nth_gprime(head,b);
			for(c = b + 1; c < list_size; ++c) {
				s2 = s1 + nth_gprime(head,c);
				for(d = c + 1; d < list_size; ++d) {
					s3 = s2 + nth_gprime(head,d);
					// Now have final sum (s3) and 4 gprimes referenced by a,b,c and d
					// Allocate, assign values and prepend
					fps_ptr = (FourPrimeSum*)malloc(sizeof(FourPrimeSum));
					fps_ptr->total = s3;
					fps_ptr->p0 = nth_gprime(head,a);
					fps_ptr->p1 = nth_gprime(head,b);
					fps_ptr->p2 = nth_gprime(head,c);
					fps_ptr->p3 = nth_gprime(head,d);
					Sums = g_slist_prepend(Sums, fps_ptr);
				} // for d ...
			} // for c ...
		} // for b ...
	} // for a ...

	// show list size
	printf("Items in Sums list = %i\n", g_slist_length(Sums));
	// now sort list by total
	Sums = g_slist_sort(Sums,compare_fps);
	printf("Items in Sums list = %i\n", g_slist_length(Sums));
	
	// Open disk file here
	FILE *fout = fopen(outfile,"wb");
	if(fout == NULL) 
	{
		printf("Error: No file opened.\n");
		exit(1);
	}
	
	// now iterate over the Sums list and print block sizes
	working = Sums;	// GSList *working
	double complex current_total = CMPLX(0.0,0.0);
	int blk_size,max_blk_size = 6;
	while(working != NULL) {
		fps_ptr = working->data;
		if(compare_gprime( &(fps_ptr->total), &current_total) != 0) { // new total
			// Mark block start
			blk_start = working;
			// update current total
			current_total = fps_ptr->total;
			blk_size = 1;
			// goto next entry in Sums list
			working = g_slist_next(working);
			while(working != NULL) {
				fps_ptr = working->data;
				if(compare_gprime( &(fps_ptr->total), &current_total) == 0) {
					++blk_size;
					working = g_slist_next(working);
				} 
				else break;
			} // while...
			if(blk_size > max_blk_size) {
				max_blk_size = blk_size;
				// working points to end-of-block + 1
				// blk_start points to head of block
				write_ptr = blk_start;
				printf("Block size: %d\n{\n",blk_size);
				while(write_ptr != working) {
					fps_ptr = write_ptr->data;
					printf("{CMPLX(%.1f,%.1f),", creal(fps_ptr->p0), cimag(fps_ptr->p0));
					printf("CMPLX(%.1f,%.1f),", creal(fps_ptr->p1), cimag(fps_ptr->p1));
					printf("CMPLX(%.1f,%.1f),", creal(fps_ptr->p2), cimag(fps_ptr->p2));
					printf("CMPLX(%.1f,%.1f)},\n",  creal(fps_ptr->p3), cimag(fps_ptr->p3));		
					write_ptr = g_slist_next(write_ptr);					
				} // while write_ptr
				printf("}\n");
				
				// ----------Code to write block to disk file here----------
				// For each block:
				//		write number of rows of 4 gprimes as integer;
				fwrite((const void*)&blk_size, sizeof(int), 1, fout);
				// set write_ptr to blk_start
				write_ptr = blk_start;
				// write rows of 4 gprimes to disk
				while(write_ptr != working) {
					fps_ptr = write_ptr->data;
					const void *p0 = &(fps_ptr->p0);
					fwrite(p0, sizeof(gprime), 4, fout);
					write_ptr = g_slist_next(write_ptr);
				}
				// ----------End block write----------
				
			} // if blk_size
		} // if new total
	} // while not NULL
	
	printf("Max Block Size = %d\n", max_blk_size);
			
	// =====Cleanup Code=====
	
	// Close Disk File here
	fclose(fout);
	printf("equalsums.dat closed.\n");
	// Free the list of gprimes
	g_slist_free_full(head,free);	
	printf("Gprimes list cleared.\n");
	g_slist_free_full(Sums,free);
	printf("Sums list cleared.\n");
	
	// =====Code to verify disk file=====
	int nBlocks = 0;
	printf("\nTesting .dat file\n\n");
	FILE *fin = fopen(outfile,"rb");
	if(fin == NULL) {
		printf("Error: Failed to open file for reading.\n");
		exit(1);
	}
	int in_blk_size;
	gprime in_buffer[4];
	fread(&in_blk_size, sizeof(int), 1, fin);
	while(!(feof(fin))) {
		++nBlocks;
		printf("Block size: %d\n", in_blk_size);
		for(int b = 0; b < in_blk_size; ++b) {
			fread(in_buffer, sizeof(gprime), 4, fin);
			//for(int a = 0; a < 4; ++a) prt_gprime(in_buffer[a]);
			//printf("\n");
		}
		fread(&in_blk_size, sizeof(int), 1, fin);
	}
	fclose(fin);
	printf("\nRead %d blocks. Test complete.\n\n", nBlocks);	
	// =====End Verify=====
	
	// =====Done=====
	return 0;
}

