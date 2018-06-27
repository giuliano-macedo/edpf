#ifndef AVL
#define AVL
#include "macros.h"
#include "base.h"
typedef struct AvlNode {
	BASE_NODE
	struct AvlNode* l;
	struct AvlNode* r;
	BaseNodeStack* s;
	BaseNodeStack* p;
	char fb;
}AvlNode;
AvlNode* avl_new();

AvlNode*       avl_newNode(BaseNode*n);
AvlNode*       avl_new();
int      	   avl_insert(AvlNode** t,BaseNode* v,int flag);
int            avl_insert2(AvlNode** t,AvlNode* n,int flag);
AvlNode* 	   avl_searchByName(AvlNode**t,char* str);	
AvlNode* 	   avl_remove(AvlNode**a,char* str,char* flag);

int      	   avl_printAge(AvlNode*t,uint age);
int     	   avl_printCompatibleDonors(AvlNode* t,BloodType bt);
int     	   avl_printBloodType(AvlNode*t,BloodType bt);

void           avl_getGalon(AvlNode* t,float* g);
void    	   avl_print(AvlNode*t);
void           avl_printNode(AvlNode*t);

#endif