/*
 * analyse_v2.h
 * 
 * Notes: 13Jan2015
 * This version of analyse saves the sorted ToctaInfo list
 * to a binary format file. The generic name is:
 * "../toctas/Tocta-%03u-analysed.toc"
 * 
 * Copyright 2014 Michael Tate <michael.tate@wanadoo.fr>
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
 * Continuing from dev01.c
 * Input a list of Toctas
 * Output a list of lists grouped by membership of the same
 * rotation/reflection group.
 * Use Glib single linked lists
 */

#include <glib.h>
#include "../include/dict_search.h"

int main(int argc, char **argv)
{	
	FILE *fp,*fout;
	GSList *ToctaInfos = NULL;
	GSList *base_tocta = NULL;
	GSList *scan = NULL;
	ToctaInfo buffer, *tip;
	int major,minor,bd,ef;
	Tocta *tocta_wrk = (Tocta*)(&buffer);
	
	if(argc != 2) {
	printf("Usage: ./analyse_v2 [Target]\n");
	exit(0);
	}	
	const uint32_t Target = (uint32_t)atoi(argv[1]);
	char fname[256];
	sprintf(fname,"../data/Tocta-%03u-consolidated.toc",Target);
	
	if((fp = fopen((char * restrict)fname, "r")) == NULL) {
		printf("Error opening file %s.\n",fname);
		exit(1);
	} else {
		printf("Opened %s for analysis.\n", fname);
	}
	
	// Construct a GSList of toctas.	
	fread(&buffer, sizeof(Tocta), 1, fp);
	while(!feof(fp)) {
		// allocate space and copy from buffer
		tip = malloc(sizeof(ToctaInfo));
		copyToctaInfo(&buffer, tip);
		tip->major = 0;
		tip->minor = 0;
		tip->target = Target;
		// prepend to GSList
		ToctaInfos = g_slist_prepend(ToctaInfos, tip);
		// read next
		fread(&buffer, sizeof(Tocta), 1, fp);
	}
	
	printf("Analysing %i elements from consolidated list...\n", g_slist_length(ToctaInfos));
	// set loop conditions
	base_tocta = ToctaInfos;
	major = 0;
	
	// outer loop - using base_tocta as pointer
	while(base_tocta != NULL) {
		major += 1;
		minor = 1;
		// copy this toctainfo to a work buffer
		copyToctaInfo(TIP(base_tocta), &buffer);
		// calc and process the first 16 rotations
		for(bd=0; bd<4; bd++) {
			for(ef=0; ef<4; ef++) {
				// find match in main list using g_slist_find_custom
				scan = g_slist_find_custom(ToctaInfos, (gconstpointer)&buffer, compareToctaInfo);
				if(scan == NULL) {
					printf("Warning: rotation not found in ToctaInfos.\n");
				} else {
					if((TIP(scan)->major == 0)&&(TIP(scan)->minor == 0)) {
						TIP(scan)->major = major;
						TIP(scan)->minor = minor;
						minor += 1;
						//printf("Main list updated.\n");
					} else {
						printf("Warning: Collision %i/%i non zero\n",TIP(scan)->major,TIP(scan)->minor);
					}
				}				
				// rotate buffer
				apply_ef(tocta_wrk);									
			} // for ef
			apply_bd(tocta_wrk);
		} // for bd
		
		// lots more rotations/reflections here
		apply_ac(tocta_wrk);

		for(ef=0; ef<4; ef++) {			
		// find match in main list using g_slist_find_custom
			scan = g_slist_find_custom(ToctaInfos, (gconstpointer)&buffer, compareToctaInfo);
			if(scan == NULL) {
				printf("Warning: rotation not found in ToctaInfos.\n");
			} else {
				if((TIP(scan)->major == 0)&&(TIP(scan)->minor == 0)) {
					TIP(scan)->major = major;
					TIP(scan)->minor = minor;
					minor += 1;
					//printf("Main list updated.\n");
				} else {
					printf("Warning: Collision %i/%i non zero\n",TIP(scan)->major,TIP(scan)->minor);
				}
			}				
			// rotate buffer
			apply_ef(tocta_wrk);			
		}
				
		apply_ac(tocta_wrk);
		apply_ac(tocta_wrk);
				
		for(ef=0; ef<4; ef++) {
			// find match in main list using g_slist_find_custom
			scan = g_slist_find_custom(ToctaInfos, (gconstpointer)&buffer, compareToctaInfo);
			if(scan == NULL) {
				printf("Warning: rotation not found in ToctaInfos.\n");
			} else {
				if((TIP(scan)->major == 0)&&(TIP(scan)->minor == 0)) {
					TIP(scan)->major = major;
					TIP(scan)->minor = minor;
					minor += 1;
					//printf("Main list updated.\n");
				} else {
					printf("Warning: Collision %i/%i non zero\n",TIP(scan)->major,TIP(scan)->minor);
				}
			}				
			// rotate buffer
			apply_ef(tocta_wrk);			
		}			
		// restore to initial config
		apply_ac(tocta_wrk);

		// reflect
		apply_abcd(tocta_wrk);

		// calc and save the next 16 rotations
		for(bd=0; bd<4; bd++) {
			for(ef=0; ef<4; ef++) {
				// find match in main list using g_slist_find_custom
				scan = g_slist_find_custom(ToctaInfos, (gconstpointer)&buffer, compareToctaInfo);
				if(scan == NULL) {
					printf("Warning: rotation not found in ToctaInfos.\n");
				} else {
					if((TIP(scan)->major == 0)&&(TIP(scan)->minor == 0)) {
						TIP(scan)->major = major;
						TIP(scan)->minor = minor;
						minor += 1;
						//printf("Main list updated.\n");
					} else {
						printf("Warning: Collision %i/%i non zero\n",TIP(scan)->major,TIP(scan)->minor);
					}
				}				
				// rotate buffer
				apply_ef(tocta_wrk);
			} // for ef
			apply_bd(tocta_wrk);
		} // for bd	
		
		apply_ac(tocta_wrk);

		for(ef=0; ef<4; ef++) {			
			// find match in main list using g_slist_find_custom
			scan = g_slist_find_custom(ToctaInfos, (gconstpointer)&buffer, compareToctaInfo);
			if(scan == NULL) {
				printf("Warning: rotation not found in ToctaInfos.\n");
			} else {
				if((TIP(scan)->major == 0)&&(TIP(scan)->minor == 0)) {
					TIP(scan)->major = major;
					TIP(scan)->minor = minor;
					minor += 1;
					//printf("Main list updated.\n");
				} else {
					printf("Warning: Collision %i/%i non zero\n",TIP(scan)->major,TIP(scan)->minor);
				}
			}				
			// rotate buffer
			apply_ef(tocta_wrk);			
		}
				
		apply_ac(tocta_wrk);
		apply_ac(tocta_wrk);
				
		for(ef=0; ef<4; ef++) {
			// find match in main list using g_slist_find_custom
				scan = g_slist_find_custom(ToctaInfos, (gconstpointer)&buffer, compareToctaInfo);
				if(scan == NULL) {
					printf("Warning: rotation not found in ToctaInfos.\n");
				} else {
					if((TIP(scan)->major == 0)&&(TIP(scan)->minor == 0)) {
						TIP(scan)->major = major;
						TIP(scan)->minor = minor;
						minor += 1;
						//printf("Main list updated.\n");
					} else {
						printf("Warning: Collision %i/%i non zero\n",TIP(scan)->major,TIP(scan)->minor);
					}
				}				
				// rotate buffer
			apply_ef(tocta_wrk);			
		}			
		
		// End Rotations and Reflections
		
		// restore to initial config
		// apply_ac(tocta_wrk);
				
		// move the base_tocta along list
		do {
			base_tocta = base_tocta->next;
			if(base_tocta == NULL) break;	// end of list
		} while((TIP(base_tocta)->major != 0)||((TIP(base_tocta)->minor != 0)));
				
	} // while(base_tocta != NULL)
	
	// sort the list of ToctaInfos by major/minor values
	ToctaInfos = g_slist_sort(ToctaInfos, compareToctaVers);
	
	// Save the modified ToctaInfos list with toctas and major/minor
	sprintf(fname,"../data/Tocta-%03u-analysed.toc",Target);
	if((fout=fopen(fname,"wb"))==NULL) {
		printf("Unable to open %s for writing.\n",fname);
		exit(1);
	}
	base_tocta = ToctaInfos;
	while(base_tocta != NULL) {
		fwrite(TIP(base_tocta), sizeof(ToctaInfo), 1, fout);
		base_tocta = base_tocta->next;
	}
	fclose(fout);

	prtToctaInfo6x8(ToctaInfos,Target);
	printf("\nSorted %i elements from ToctaInfos into %i groups.\n", g_slist_length(ToctaInfos), g_slist_length(ToctaInfos)/48 );
	fclose(fp);
	return 0;
}
