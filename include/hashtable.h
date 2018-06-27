#ifndef HASHTABLE
#define HASHTABLE
#include <stdint.h>
#include "macros.h"
#include "base.h"
typedef struct HashTable{
    BaseNodeStack** bucket; //vetor de pilhas
    
    uint32_t mod;
    uint32_t mask;
}HashTable;
HashTable* hashtable_new();
void hashtable_insert(HashTable* ht,BaseNode* v);
void hashtable_setSize(HashTable* ht,uint n);
uint32_t hashtable_hashfunc(HashTable* ht,char* k);

#endif