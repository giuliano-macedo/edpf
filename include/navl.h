#ifndef NAVL
#define NAVL
#include "macros.h"
#include "base.h"

typedef struct NavlNode{
	BASE_NODE
	//..
}NavlNode;
NavlNode* navl_new();
void navl_insert(NavlNode** ht,BaseNode* n);
#endif