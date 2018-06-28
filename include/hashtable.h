#ifndef HASHTABLE
#define HASHTABLE
#include <stdint.h>
#include "macros.h"
#include "base.h"
typedef struct Destack{
	union{
		uint32_t k;
		BaseNode* b;
	}v;
	struct Destack* n;
	struct Destack* p;
	struct Destack* bk; //so uso ese campo quando estiver no bucket
}Destack;
typedef struct HashTable{
    Destack** bucket; //vetor de pilhas
    uint32_t mod;
    uint32_t mask;
    uint32_t size;
    uint noColisions;
    Destack* keys;//pilha de chaves
}HashTable;
HashTable* hashtable_new();
void 	   hashtable_insert(HashTable* ht,BaseNode* v);
BaseNode*  hashtable_remove(HashTable* ht,char*v);
void	   hashtable_setSize(HashTable* ht,uint n);
uint32_t   hashtable_hashfunc(HashTable* ht,char* k);
void 	   hashtable_print(HashTable* ht);
int 	   hashtable_printAge(HashTable* ht,uint age);
int 	   hashtable_printBloodType(HashTable*ht,BloodType bt);
void 	   hashtable_getGalon(HashTable*ht,float* galon);
int 	   hashtable_printCompatibleDonors(HashTable*ht,BloodType bt);
BaseNode*  hashtable_searchByName(HashTable*ht,char* name);


void destack_pushKey(Destack** s,uint32_t v);
void destack_pushValue(Destack** s,BaseNode* v);
void destack_remove(Destack** s);

#endif