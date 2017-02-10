/*
 *  Investigation of a network called a TateOctahedron.
 * 	Derived from a discussion of a Stewart Cube network in John Cooke's blog.
 *  http://www.johndcook.com/blog/2013/08/23/stewarts-cube/
 * 
 *  ==========Version Posix Threads==========
 * 
    Copyright (C) 2013  Michael J Tate.	<michael.tate@wanadoo.fr>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 */

#ifndef __TOCTA__
#define __TOCTA__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <pthread.h>

#define PRIMES "./Primes.int"
#define NPTR(gl) ((struct node4 *)((gl)->data))
#define RPTR(gl) ((struct ring4 *)((gl)->data))
#define OPTR(gl) ((struct octahedron *)((gl)->data))
#define GPTR(gl) ((struct group *)((gl)->data))
#define STPTR(p) ((struct tocta*)(p->data))

#define E_SUM  (newring->nodes[0]->primes[2]+\
				newring->nodes[1]->primes[2]+\
				newring->nodes[2]->primes[2]+\
				newring->nodes[3]->primes[2])
				
#define F_SUM  (newring->nodes[0]->primes[3]+\
				newring->nodes[1]->primes[3]+\
				newring->nodes[2]->primes[3]+\
				newring->nodes[3]->primes[3])				
	

struct node4 {
	int node4_id;
	int primes[4];
};

struct ring4 {
	struct node4 *nodes[4];
};


struct octahedron {
	struct ring4 *pRing;
	struct node4 *VertexNodes[2];
	int Target;
	int Signature[12];	// unique identity of octahedron. Can be rebuilt from this.
	int Group[12];		// identity sorted in ascending order	
};


struct tocta {
	// Used internally by searchGroups	
	int tocta_id;
	int primes[16];
	int nodes[6];
	char id_str[256];
	int flag;
	int target;
	int group;
	int Group[12];		// identity sorted in ascending order	
};

struct thread_info_block {
	pthread_t thread_id;
	int target;
	int start_idx;
	int n_items;
	GSList *p_GSL, **NodeList;
	int *NodePairs;
	int n_nodes;
};


void searchOcta(int);
void* searchOcta_pthread(void *data);

int setupPrimeLinkedList(GSList **, int);
int searchNodesLinkedList(GSList **, GSList **, int);
int searchRingsLinkedList(GSList **, GSList **);

int searchRingsArray(GSList **, GSList **, int);
int searchRingsArray_pthread(GSList **, GSList **, int);

int searchOctahedron(GSList **, GSList **, GSList**, int);
int searchOctahedronSortedArray(GSList **Nodes, GSList **Rings, GSList **Octahedrons, int Target);
int searchGroup_px(GSList **Octahedrons, GSList **Groups, int Target);

void printNode4(struct node4 *np);
void printRing4_compact(struct ring4 *rp);
void printOctahedron_compact(struct octahedron *op);
int adjacentNode4(struct node4 *, struct node4 *);
int diagonalNode4(struct node4 *, struct node4 *);
int compare_int(const void *a, const void *b);
int compare_node4(const void *l, const void *r);

gint find_custom_id_str(gconstpointer, gconstpointer);
void prt_struct_tocta(struct tocta *);

void file_prt_struct_tocta(FILE *, struct tocta *, char *);
char* calc_rotated_id_str(struct tocta *, int, char *);

void apply_abcd(struct tocta *t);
void apply_ef(struct tocta *);
void apply_bd(struct tocta *);
void apply_ac(struct tocta *);

// Test Function
void find_and_flag(struct tocta *working, GSList *Base, GSList *pBL_curr, GSList **Sublist);

#endif


