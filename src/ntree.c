#include "ntree.h"
NtreeNode* ntree_new(){
	return (NtreeNode*)calloc(1,sizeof(NtreeNode));
}
void ntree_ainsert(TreeNode** t,BaseNode* n){
	if(!*t){
		*t=tree_newNode(n);
		return;
	}
	uint a=n->age,b=(*t)->age;
	if(a>b){
		ntree_ainsert(&(*t)->r,n);
	}
	else if(a<b){
		ntree_ainsert(&(*t)->l,n);
	}
	else{
		basenodestack_push(&(*t)->s,n);

	}
}
void ntree_binsert(TreeNode** t,BaseNode* n){
	if(!*t){
		*t=tree_newNode(n);
		return;
	}
	BloodType a=n->bt,b=(*t)->bt;
	if(a>b){
		ntree_binsert(&(*t)->r,n);
	}
	else if(a<b){
		ntree_binsert(&(*t)->l,n);
	}
	else{
		basenodestack_push(&(*t)->s,n);
	}
}
void ntree_insert(NtreeNode* nt,BaseNode* n){
	tree_insert(&nt->tn,n);
	ntree_ainsert(&nt->ta,n);
	ntree_binsert(&nt->tb,n);
}

NtreeNode** ntree_searchByName(NtreeNode*nt,char* str){
	return 0;
}
void ntree_removeNode(NtreeNode*nt){

}

int ntree_printAge(NtreeNode*t,uint age){
	return 0;
}
int ntree_printCompatibleDonors(NtreeNode* t,BloodType bt){
	return 0;
}
int ntree_printBloodType(NtreeNode*t,BloodType bt){
	return 0;
}

void ntree_getGalon(NtreeNode* t,float* g){

}
void ntree_print(NtreeNode*t){

}