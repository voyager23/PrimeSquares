/*
 *  Investigation of a network called a TateOctahedron.
 * 	Derived from a discussion of a Stewart Cube network in John Cooke's blog.
 *  http://www.johndcook.com/blog/2013/08/23/stewarts-cube/
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

#include "../pthreads/tocta.h"

//----------------------------------------------------------------------
void apply_abcd(struct tocta *t) {
	// apply reflection about horizontal plane abcd
	// modifies data pointed to by t
	int primes[16];
	int i,j;
	char scratch[64];
	// record the new config in array
	primes[0]=t->primes[0];
	primes[1]=t->primes[1];
	primes[2]=t->primes[3];
	primes[3]=t->primes[2];
	
	primes[4]=t->primes[4];
	primes[5]=t->primes[5];
	primes[6]=t->primes[7];
	primes[7]=t->primes[6];
	
	primes[8]=t->primes[8];
	primes[9]=t->primes[9];
	primes[10]=t->primes[11];
	primes[11]=t->primes[10];
	
	primes[12]=t->primes[12];
	primes[13]=t->primes[13];
	primes[14]=t->primes[15];
	primes[15]=t->primes[14];
	// copy back to source tocta
	for(i=0;i<16;i++) t->primes[i] = primes[i];
	// update the id_str
	for(j=0;j<16;j++) {
		if(j==0) {
			sprintf(scratch,"%d",t->primes[j]);
			strcpy(t->id_str,scratch);
		} else {
			sprintf(scratch,"%d",t->primes[j]);
			strcat(t->id_str,scratch);
		}
	}
	// set nodes to -1 since these are not correct
	// for(j=0;j<6;j++) t->nodes[j] = -1;
	// set tocta_id to 0
	t->tocta_id = 0;
	// apply_abcd
}

//----------------------------------------------------------------------
void apply_ef(struct tocta *t) {
	// apply rotation around vertical e-f axis
	// modifies the data pointed to by t
	int primes[16];
	int i,j;
	char scratch[64];
	// record the new config in array
	primes[0]=t->primes[9];
	primes[1]=t->primes[0];
	primes[2]=t->primes[14];
	primes[3]=t->primes[15];
	
	primes[4]=t->primes[0];
	primes[5]=t->primes[1];
	primes[6]=t->primes[2];
	primes[7]=t->primes[3];
	
	primes[8]=t->primes[1];
	primes[9]=t->primes[5];		
	primes[10]=t->primes[6];
	primes[11]=t->primes[7];
	
	primes[12]=t->primes[5];	// error?
	primes[13]=t->primes[9];
	primes[14]=t->primes[10];
	primes[15]=t->primes[11];
	// copy back to source tocta
	for(i=0;i<16;i++) t->primes[i] = primes[i];
	// update the id_str
	for(j=0;j<16;j++) {
		if(j==0) {
			sprintf(scratch,"%d",t->primes[j]);
			strcpy(t->id_str,scratch);
		} else {
			sprintf(scratch,"%d",t->primes[j]);
			strcat(t->id_str,scratch);
		}
	}
	// set nodes to -1 since these are not correct
	// for(j=0;j<6;j++) t->nodes[j] = -1;
	// set tocta_id to 0
	t->tocta_id = 0;
	// apply_ef	
}
//----------------------------------------------------------------------
void apply_bd(struct tocta *t) {
	// apply rotation around diagonal b-d axis
	// modifies the data pointed to by t
	int primes[16];
	int i,j;
	char scratch[64];
	// record the new config in array	
	primes[0]=t->primes[15];
	primes[1]=t->primes[7];
	primes[2]=t->primes[3];
	primes[3]=t->primes[11];
	
	primes[4]=t->primes[7];
	primes[5]=t->primes[6];
	primes[6]=t->primes[1];
	primes[7]=t->primes[5];
	
	primes[8]=t->primes[6];
	primes[9]=t->primes[14];
	primes[10]=t->primes[2];
	primes[11]=t->primes[10];
	
	primes[12]=t->primes[14];
	primes[13]=t->primes[15];
	primes[14]=t->primes[0];
	primes[15]=t->primes[9];	
	// copy back to source tocta
	for(i=0;i<16;i++) t->primes[i] = primes[i];
		// update the id_str
	for(j=0;j<16;j++) {
		if(j==0) {
			sprintf(scratch,"%d",t->primes[j]);
			strcpy(t->id_str,scratch);
		} else {
			sprintf(scratch,"%d",t->primes[j]);
			strcat(t->id_str,scratch);
		}
	}
	// set nodes to -1 since these are not correct
	// for(j=0;j<6;j++) t->nodes[j] = -1;
	// set tocta_id to 0
	t->tocta_id = 0;
	// apply_bd
}
//----------------------------------------------------------------------
void apply_ac(struct tocta *t) {
	// apply rotation around diagonal a_c axis
	// modifies the data pointed to by t
	int primes[16];
	int i,j;
	char scratch[64];
	// record the new config in array	
	primes[0]=t->primes[3];
	primes[1]=t->primes[2];
	primes[2]=t->primes[0];
	primes[3]=t->primes[1];
	
	primes[4]=t->primes[2];
	primes[5]=t->primes[10];
	primes[6]=t->primes[14];
	primes[7]=t->primes[6];
	
	primes[8]=t->primes[10];
	primes[9]=t->primes[11];
	primes[10]=t->primes[9];
	primes[11]=t->primes[5];
	
	primes[12]=t->primes[11];
	primes[13]=t->primes[3];
	primes[14]=t->primes[15];
	primes[15]=t->primes[7];	
	// copy back to source tocta
	for(i=0;i<16;i++) t->primes[i] = primes[i];
		// update the id_str
	for(j=0;j<16;j++) {
		if(j==0) {
			sprintf(scratch,"%d",t->primes[j]);
			strcpy(t->id_str,scratch);
		} else {
			sprintf(scratch,"%d",t->primes[j]);
			strcat(t->id_str,scratch);
		}
	}
	// set nodes to -1 since these are not correct
	// for(j=0;j<6;j++) t->nodes[j] = -1;
	// set tocta_id to 0
	t->tocta_id = 0;
	// apply_ac
}	
//----------------------------------------------------------------------

void prt_struct_tocta(struct tocta *p) {
	// version: print to stdout
	int i;	
	if(p==NULL) return;
	printf("\nTarget: %3d  Group: %3d  Ref: %3d\n",p->target,p->group,p->tocta_id);
	printf("Primes: ");
	for(i=0; i<16; i++) {
		printf("%d", p->primes[i]);
		if(i!=15) printf(",");
		if(((i+1)%4)==0) printf("  ");
	}
	printf("\nId_str: %s\n", p->id_str);
}

//----------------------------------------------------------------------

void file_prt_struct_tocta(FILE *fout, struct tocta *p, char *option_char) {
	// version: print to file
	int i;
	if(p==NULL) return;
	if(strcmp(option_char, ">")==0) {
		fprintf(fout,"\nTarget: %3d  Group: %3d  Ref: %3d\n",p->target,p->group,p->tocta_id);
		fprintf(fout,"Primes%s ", option_char);
		for(i=0; i<16; i++) {
			fprintf(fout,"%d", p->primes[i]);
			if(i!=15) fprintf(fout,",");
			if(((i+1)%4)==0) fprintf(fout,"  ");
		}
	#if(0)
		fprintf(fout,"\nId_str: %s\n", p->id_str);
		// print group_id
		fprintf(fout,"GrpArray%s ", option_char);
		for(i=0; i<12; i++) fprintf(fout,"%02d ", p->Group[i]);

	#endif
		// more new code

		fprintf(fout,"\n");
		fprintf(fout,"Nodes%s ", option_char);
		for(i=0; i<6; i++) fprintf(fout,"%03d ", p->nodes[i]);
		fprintf(fout,"\n");
	}
}

//----------------------------------------------------------------------
gint find_custom_id_str(gconstpointer l, gconstpointer target_str) {
	/*
	 * First parameter is void * from data in GList element.
	 * Second paramter is pointer to search string
	 */
	 struct tocta *a = (struct tocta*)(l);
	 return strcmp(a->id_str, (const char*)(target_str));
 }
//----------------------------------------------------------------------

int compare_int(const void *a, const void *b) {
	if(*(int*)a < *(int*)b) return -1;
	if(*(int*)a == *(int*)b) return 0;
	return 1;
}

//------------------------------------------------------------------//

int compare_node4(const void *l, const void *r) {
	int i;
	struct node4 *p = (struct node4*)(l);
	struct node4 *q = (struct node4*)(r);
	for(i=0; i<4; i++) {
		if(	(p->primes[i])<(q->primes[i])) return -1;
		if(	(p->primes[i])>(q->primes[i])) return  1;
	}		
	return 0;
}

//------------------------------------------------------------------//

void printNode4(struct node4 *np) {
	if(np != NULL) {
			printf("  node_id = %03d    p0 = %03d  p1 = %03d  p2 = %03d  p3 = %03d\n", 
				np->node4_id,np->primes[0],np->primes[1],np->primes[2],np->primes[3] );
		} else {
			printf("NULL node pointer.\n");
		}
}

//------------------------------------------------------------------//

void printRing4_compact(struct ring4 *rp) {
	int i;
	for(i=0; i<4; i++) printNode4(rp->nodes[i]);
	
	
}
//------------------------------------------------------------------//


void printOctahedron_compact(struct octahedron *op) {
	
	printf("\n================== Target %3d ================\n",op->Target);
	printNode4(op->VertexNodes[0]);
	printf("----------------Upper Node (p2)---------------\n");
	printRing4_compact(op->pRing);
	printf("----------------Lower Node (p3)---------------\n");
	printNode4(op->VertexNodes[1]);
#if(0)
	int i;
	printf("-------------------Signature------------------\n\t");
	for(i=0; i<12; i++) printf("%02d ", op->Signature[i]);	printf("\n");
	printf("---------------------Group--------------------\n\t");
	for(i=0; i<12; i++) printf("%02d ", op->Group[i]);		printf("\n");
#endif
	printf("==============================================\n");
}

	
int adjacentNode4(struct node4 *a, struct node4 *b) {
	// The order of the paramters is important.
	// By convention, node a/p1 => node b/p0.
	// If the adjacent nodes comply then return 1
	// else return 0 failure.
	int i;
	if(a->primes[1] != b->primes[0]) return 0;
	for(i=1; i<4; i++) {
		if( (a->primes[0] == b->primes[i])||
			(a->primes[2] == b->primes[i])||
			(a->primes[3] == b->primes[i]) ) 
		return 0;
	}	
	return 1;
}
//------------------------------------------------------------------//

int diagonalNode4(struct node4 *a, struct node4 *b) {
	// The order of the paramters is important.
	// By convention, node a comes before node b
	// For diagonal nodes, all prime weights must be unique
	// If the diagonal nodes comply then return 1
	// else return 0 failure.
	int i;
	for(i=0; i<4; i++) {
		if( (a->primes[0] == b->primes[i])||
			(a->primes[1] == b->primes[i])||
			(a->primes[2] == b->primes[i])||
			(a->primes[3] == b->primes[i]) )
		return 0;
	}	
	return 1;
}
//------------------------------------------------------------------//

