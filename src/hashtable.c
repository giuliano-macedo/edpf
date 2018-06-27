#include "hashtable.h"
void hashtable_insert(HashTable* ht,BaseNode* v){
	uint32_t h=hashtable_hashfunc(ht,v->name);
	// ht->keys[ht->size++]=h;
	basenodestack_push(&ht->bucket[h],basenode_copy(v));
}
HashTable* hashtable_new(){
	HashTable* ans=(HashTable*)calloc(1,sizeof(HashTable));
	ans->mask=0xf1024ad0;//mascara aleatória
	return ans;
}
void hashtable_setSize(HashTable* ht,uint n){
	ht->mod=n;
	ht->bucket=(BaseNodeStack**)calloc(n,sizeof(BaseNodeStack*));
	// ht->keys=(uint32_t*)calloc(n,sizeof(uint32_t));
	// ht->size=0;
}
uint32_t hashtable_hashfunc(HashTable* ht,char* k){
	uint32_t ans=0;
	char* temp=(char*)(&ans);
	char c;
	uint i=0;
	while((k[i++])){
		temp[i%4]+=k[i];
	}
	return (ans^ht->mask)%ht->mod;
}