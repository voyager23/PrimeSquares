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
	// main loop - expects a SigTrans *data to base configuration
	int index = 0;
	for(int abcd = 0; abcd < 2; ++abcd) {
		for(int bd = 0; bd < 4; ++bd) {
			for(int ef = 0; ef < 4; ++ef) {
				prt_sigtrans(data, index++ );	// write to list
				data = gt_apply_ef(data);
				if(data == NULL) {
					printf("ef returned NULL.\n");
					exit(1);
				}			
			} // for ef...
			data = gt_apply_bd(data);
			if(data == NULL) {
				printf("bd returned NULL.\n");
				exit(1);
			} 			
		} // for bd...
		
		data = gt_apply_ac(data);
		
		for(int ef = 0; ef < 4; ++ef) {
			prt_sigtrans(data, index++ );	// write to list
			data = gt_apply_ef(data);
			if(data == NULL) {
				printf("ef returned NULL.\n");
				exit(1);
			}			
		} // for ef...
		
		data = gt_apply_ac(data);
		data = gt_apply_ac(data);
		for(int ef = 0; ef < 4; ++ef) {
			prt_sigtrans(data, index++ );	// write to list
			data = gt_apply_ef(data);
			if(data == NULL) {
				printf("ef returned NULL.\n");
				exit(1);
			}			
		} // for ef...
		// Restore initial config - have 24 configs
		data = gt_apply_ac(data);
		// reflect
		data = gt_apply_abcd(data);
	} // end main loop
	
	// confirm this equals first config
	prt_sigtrans(data,999);
	// Cleanup code
	free(data);	
}

