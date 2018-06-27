#ifndef TREE
#define TREE
#include "base.h"
#include "macros.h"
typedef struct TreeNode {
	BASE_NODE
	struct TreeNode* l;
	struct TreeNode* r;
	BaseNodeStack* s;
	//..
}TreeNode;
TreeNode*  tree_newNode(BaseNode*n);
TreeNode*  tree_new();
void 	   tree_insert(TreeNode** ht,BaseNode* n);
void 	   tree_insert2(TreeNode** t,TreeNode* n);
TreeNode** tree_searchByName(TreeNode**t,char* str);	
void 	   tree_removeNode(TreeNode**n);

int  	   tree_printAge(TreeNode*t,uint age);
int 	   tree_printCompatibleDonors(TreeNode* t,BloodType bt);
int 	   tree_printBloodType(TreeNode*t,BloodType bt);

void       tree_getGalon(TreeNode* t,float* g);
void 	   tree_print(TreeNode*t);
void       tree_printNode(TreeNode*t);
#define FORNTREE
#endif