#ifndef BBMED
#define BBMED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "macros.h"
#include "base.h"
#include "tree.h"
#include "ntree.h"
#include "avl.h"
#include "navl.h"
#include "hashtable.h"
BloodType parseType(char* str);
typedef union DataBase{ //bd pode ser um desses tipos
	TreeNode* t;
	NtreeNode* nt;
	AvlNode* a;
	NavlNode* na;
	HashTable* h;
}DataBase;
void err(char* msg);
void bd_init(DataBase* bd,DataBaseType dt);
uint bd_read(DataBase* b,FILE*bdfile,DataBaseType dt);
#endif