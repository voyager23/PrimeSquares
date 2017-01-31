/*
 * group_toctas.c
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


#include "ps1.h"

typedef struct sigtrans {
	Signature signature;
	Matrix transpose;
}SigTrans;

int main(int argc, char **argv)
{
	char *fname26 = "blocks/Tocta-12-26.blk";	// 144 configs (3 groups)
	char *fname28 = "blocks/Tocta-12-28.blk";	// 192 configs (4 groups)
	GSList *working, *inlist = NULL;
	SigTrans *st;
	FILE *fin;
	
	fin = fopen(fname26,"rb");
	if(fin == NULL) {
		printf("Unable to open %s ... stopping.\n", fname26);
		exit(1);
	}
	
	do {
		st = (SigTrans*)malloc(sizeof(SigTrans));
		fread(&(st->transpose), sizeof(Matrix), 1, fin);
		if(feof(fin)) {
			free(st);
		} else {
		inlist = g_slist_prepend(inlist, st);
		}
	} while(!(feof(fin)));	
	fclose(fin);
	
	// for each entry in inlist
	//		read 12 gprimes from the transpose in signature
	//		qsort signature (ascending)
	working = inlist;
	while(working != NULL) {
		SigTrans *stp = working->data;
		int sig_idx = 0;
		for(int row = 0; row < 4; ++row) {
			for(int col = 1; col < 4; ++col) {
				stp->signature[sig_idx] = stp->transpose[row][col];
			}
		}
		working = working->next;
	}
	
	

	
	
	g_slist_free_full(inlist, free);
	return 0;
}

