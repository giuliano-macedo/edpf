#include "base.h"
#include "macros.h"
#ifdef FORNTREE
#ifndef NTREE
#define NTREE
typedef struct NtreeNode {
	TreeNode* tn; //arvore chaveada com nome
	TreeNode* ta; //arvore chaveada com idade
	TreeNode* tb; //arvore chaveada com tipo sanguíneo
}NtreeNode;
NtreeNode*  ntree_new();

void 	    ntree_insert(NtreeNode* nt,BaseNode* n);

NtreeNode** ntree_searchByName(NtreeNode*nt,char* str);	
void 	    ntree_removeNode(NtreeNode*nt);

int  	    ntree_printAge(NtreeNode*t,uint age);
int 	    ntree_printCompatibleDonors(NtreeNode* t,BloodType bt);
int 	    ntree_printBloodType(NtreeNode*t,BloodType bt);

void        ntree_getGalon(NtreeNode* t,float* g);
void 	    ntree_print(NtreeNode*t);
#endif
#endif