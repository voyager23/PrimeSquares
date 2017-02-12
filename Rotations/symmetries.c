/*
 * symmetries.c
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

int main(int argc, char **argv)
{
	// Establish a SigTrans for testing;
	SigTrans *data;	 
	data = (SigTrans*)malloc(sizeof(SigTrans));
	
	data = init_sigtrans(data);
	if(data == NULL) {
		printf("init_sigtrans returned NULL.\n");
		exit(1);
	}

	prt_sigtrans(data, 0);
	
	data = gt_apply_abcd(data);
	
	prt_sigtrans(data, 0);
	
	
	// Cleanup code
	free(data);	
}

