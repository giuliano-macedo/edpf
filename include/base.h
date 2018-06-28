#ifndef BASE
#define BASE
#include "macros.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "ctype.h"
typedef unsigned int uint;
typedef enum DataBaseType{
	nulldbtype=0,
	tree,
	ntree,
	navl,
	avl,
	hashtable
}DataBaseType;
typedef enum BloodType{
	nullbloodtype=0,
	apos,
	bpos,
	abpos,
	opos,
	aneg,
	bneg,
	abneg,
	oneg
}BloodType;
typedef struct BaseNode{
	BASE_NODE
}BaseNode;
typedef struct BaseNodeStack{
	BaseNode*v;
	struct BaseNodeStack* n;
}BaseNodeStack;
void basenodestack_push(BaseNodeStack** s,BaseNode* v);
BaseNode* basenodestack_pop (BaseNodeStack** s);
void basenode_print(BaseNode* n);
BaseNode* basenode_copy(BaseNode* n);
void bloodTypeToString(BloodType bt,char* str);
void galon_print(float*gal);
int bloodtype_iscompatible(BloodType x,BloodType y);
uint countValidLines(FILE* f);
void err(char* msg);
#include "tree.h"
#include "ntree.h"
#include "avl.h"
#include "navl.h"
#include "hashtable.h"

#endif