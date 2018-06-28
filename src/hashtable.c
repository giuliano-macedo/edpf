#include "hashtable.h"
uint32_t hashtable_hashfunc(HashTable* ht,char* k){
	uint32_t ans=0;
	uint8_t* temp=(uint8_t*)(&ans);
	
	uint i=0;
	uint len=strlen(k);
	len*=(len%2);
	while((k[i++])){
		temp[i%4]+=k[i]+(len*((i%2)-1));
	}
	return (ans^ht->mask)%ht->mod;
}

void destack_pushKey(Destack** s,uint32_t v){
	Destack *n=(Destack*)calloc(1,sizeof(Destack));
    n->v.k=v;
    if(!*s)*s=n;
    else{
        n->n=*s;
        if((*s)->n)(*s)->n->p=n;
        *s=n;
    }
}
void destack_pushValue(Destack** s,BaseNode* v){
	Destack *n=(Destack*)calloc(1,sizeof(Destack));
    n->v.b=v;
    if(!*s)*s=n;
    else{
        n->n=*s;
        if((*s)->n)(*s)->n->p=n;
        *s=n;
    }
}
void destack_remove(Destack** s){
	Destack* temp=*s;
	if((*s)->n&&(*s)->p){//meio
		(*s)->p->n = (*s)->n;
		(*s)->n->p = (*s)->p;
	}
	else if((*s)->n){//começo
		(*s)->n->p=0;
		*s=(*s)->n;
	}
	else if((*s)->p){//fim
		(*s)->p->n=0;
	}
	else{
		free(*s);
		*s=0;
		return;
	}
	free(temp);
}
//remover hashkey antigo
void hashtable_removeKey(HashTable* ht,uint32_t k){
	Destack** s=&ht->keys;
	if(!*s)return;
	Destack* aux=*s;
	do{
		if(k==aux->v.k){
			Destack*temp=aux->n;
			if(aux==*s) destack_remove(s);
			else destack_remove(&aux);
			aux=temp;
			break;
		}
		if(aux)aux=aux->n;
	}while(aux);
}
BaseNode* hashtable_remove(HashTable* ht,char* v){
	uint32_t k=hashtable_hashfunc(ht,v);
	Destack** s=&(ht->bucket[k]);
	BaseNode* ans=0;
	if(!*s)return 0;
	Destack *aux=*s;
	do{
		if(!strcmp(v,aux->v.b->name)){
			ans=aux->v.b;
			Destack*temp=aux->n;
			if(!aux->n)destack_remove(&aux->bk);//se for o ultimo a ser removido
			else ht->noColisions--;
			if(aux==*s) destack_remove(s);
			else destack_remove(&aux);
			
			aux=temp;
		}
		if(aux)aux=aux->n;
	}while(aux);
	ht->size--;
	return ans;
}
void hashtable_insert(HashTable* ht,BaseNode* v){
	uint32_t h=hashtable_hashfunc(ht,v->name);
	Destack** value=&ht->bucket[h];
	destack_pushValue(value,basenode_copy(v));
	if(!(*value)->n){//se nao houver colisao, adicione chave
		destack_pushKey(&ht->keys,h);
		(*value)->bk=ht->keys;
	}
	else{
		ht->noColisions++;
		(*value)->bk=(*value)->n->bk;
	}
	ht->size++;
}
BaseNode* hashtable_searchByName(HashTable* ht,char* name){
	uint32_t k = hashtable_hashfunc(ht,name);
	Destack* v = ht->bucket[k];
	if(!v)return 0;
	int flag=0;
	BaseNode* ans=0;
	do{
		if(!strcmp(v->v.b->name,name)){
			if(!flag){
				ans=v->v.b;
			}
			else{
				#ifndef BENCH
				basenode_print(v->v.b);
				#endif
			}
			flag=1;
		}
		v=v->n;
	}while(v);
	return ans;
}
int hashtable_printCompatibleDonors(HashTable*ht,BloodType bt){
	Destack* keys=ht->keys;
	Destack* values;
	int ans=0;
	while(keys){
		values=ht->bucket[keys->v.k];
		while(values){
			if(bloodtype_iscompatible(bt,values->v.b->bt)){
				ans=1;
				basenode_print(values->v.b);
			}
			values=values->n;
		}
		keys=keys->n;
	}
	return ans;
}
int hashtable_printBloodType(HashTable*ht,BloodType bt){
	Destack* keys=ht->keys;
	Destack* values;
	int ans=0;
	while(keys){
		values=ht->bucket[keys->v.k];
		while(values){
			if(values->v.b->bt==bt){
				ans=1;
				basenode_print(values->v.b);
			}
			values=values->n;
		}
		keys=keys->n;
	}
	return ans;

}
int hashtable_printAge(HashTable* ht,uint age){
	Destack* keys=ht->keys;
	Destack* values;
	int ans=0;
	while(keys){
		values=ht->bucket[keys->v.k];
		while(values){
			if(values->v.b->age==age){
				ans=1;
				basenode_print(values->v.b);
			}
			values=values->n;
		}
		keys=keys->n;
	}
	return ans;
}
void hashtable_getGalon(HashTable*ht,float* g){
	Destack* keys=ht->keys;
	Destack* values;
	while(keys){
		values=ht->bucket[keys->v.k];
		while(values){
			BaseNode* v=values->v.b;
			g[v->bt]+=v->dntbld;		
			values=values->n;
		}
		keys=keys->n;
	}
}
void hashtable_print(HashTable* ht){
	Destack* keys=ht->keys;
	Destack* values;
	while(keys){
		values=ht->bucket[keys->v.k];
		while(values){
			basenode_print(values->v.b);
			values=values->n;
		}
		keys=keys->n;
	}
}
HashTable* hashtable_new(){
	HashTable* ans=(HashTable*)calloc(1,sizeof(HashTable));
	ans->mask=0xf1024ad0;//mascara aleatória
	return ans;
}
void hashtable_setSize(HashTable* ht,uint n){
	if(!n)n=64;
	ht->mod=n*(1.6666666);//sempre alocar 1/3 a mais do que pedido
	ht->bucket=(Destack**)calloc(ht->mod,sizeof(Destack*));
	ht->keys=(Destack*)calloc(ht->mod,sizeof(Destack));
	ht->size=0;
}