#include "avl.h"
AvlNode* avl_new(){
	return NULL;
}
AvlNode* avl_newNode(BaseNode*n){
	AvlNode* ans=(AvlNode*)calloc(1,sizeof(AvlNode));
	if(!n)return ans;
	strcpy(ans->name,n->name);
	ans->age=n->age;
	ans->bt=n->bt;
	ans->dntbld=n->dntbld;
	return ans;
}
BaseNode* avl_tobase(AvlNode*a){
	BaseNode* ans=(BaseNode*)calloc(1,sizeof(BaseNode));
	strcpy(ans->name,a->name);
	ans->age=a->age;
	ans->bt=a->bt;
	ans->dntbld=a->dntbld;
	return ans;
}
int avl_printAge(AvlNode*t,uint age){
	if(!t)return 0;
	BaseNodeStack* s=t->s;
	int ans=0;
	while(s){
		if(s->v->age==age){
			basenode_print(s->v);
		}
		s=s->n;
	}
	if(t->age==age){
		avl_printNode(t);
		ans=1;
	}
	if(avl_printAge(t->l,age))ans=1;
	if(avl_printAge(t->r,age))ans=1;
	return ans;
}
AvlNode* avl_searchByName(AvlNode**t,char* str){
	if(!*t) return NULL;
	uint spn=strspn(str,(*t)->name);
	if(!strcmp((*t)->name,str)){
		if((*t)->s){
			#ifndef BENCH
			printf("Cadastros com mesmo nome detectado!\n");
			#endif
			BaseNodeStack*s=(*t)->s;
			while(s){
				basenode_print(s->v);
				s=s->n;
			}
		}
		return *t;
	}
	if((str[spn]>(*t)->name[spn])){
		return avl_searchByName(&(*t)->r,str);
	}
	return avl_searchByName(&(*t)->l,str);
}
int avl_height(AvlNode*a){
	if(!a)return 0;
	int hl=avl_height(a->l)+1;
	int hr=avl_height(a->r)+1;
	if(hl>hr)return hl;
	return hr;
}
void avl_nodecopy(AvlNode*x,AvlNode*y){
	strcpy(x->name,y->name);
	x->age=y->age;
	x->bt=y->bt;
	x->dntbld=y->dntbld;
}
void avl_rotleft(AvlNode** r){
	AvlNode *x,*y,
	// *z,
	// *t1,
	*t2;
	// *t3,
	// *t4;

	x=(*r);
	y=x->r;
	// z=y->r;

	// t1=x->l;
	t2=y->l;
	// t3=z->l;
	// t4=z->r;

	*r=y;
	y->l=x;
	// x->l=t1;
	x->r=t2;
	// y->r=z;
	// z->l=t3;
	// z->r=t4;
	
	// NewBal(A) = OldBal(A) - 1 - max(OldBal(B), 0)
	// NewBal(B) = OldBal(B) - 1 + min(NewBal(A), 0)

	x->fb=x->fb -1 - (y->fb>0?y->fb:0);
	y->fb=y->fb -1 + (x->fb<0?x->fb:0);


}
void avl_rotright(AvlNode** r){
	
	AvlNode *x,*y,*t3;
	x=(*r);
	y=x->l;
	t3=y->r;

	*r=y;

	y->r=x;
	x->l=t3;

	// NewBal(A) = OldBal(A) + 1 - min(OldBal(B), 0)
	// NewBal(B) = OldBal(B) + 1 + max(NewBal(A), 0)
	x->fb=x->fb +1 - (y->fb<0?y->fb:0);
	y->fb=y->fb +1 + (x->fb>0?x->fb:0);
}
void avl_rotrightleft(AvlNode** x){
	avl_rotleft(&(*x)->l);
	avl_rotright(x);	
}
void avl_rotleftright(AvlNode** x){
	avl_rotright(&(*x)->r);
	avl_rotleft(x);
}
void avl_getGalon(AvlNode* t,float* g){
	if(!t) return;
	if(t->s){
		BaseNodeStack*s=t->s;
		while(s){
			g[s->v->bt]+=s->v->dntbld;
			s=s->n;
		}
	}
	g[t->bt]+=t->dntbld;
	avl_getGalon(t->r,g);
	avl_getGalon(t->l,g);
}
int avl_insert(AvlNode** a,BaseNode* v,int flag){
	if(!*a){
		*a=avl_newNode(v);
		return 0;
	}
	uint spn;
	char x,y;
	spn=strspn(v->name,(*a)->name);
	x=v   ->name[spn];
	y=(*a)->name[spn];
	if(!strcmp((*a)->name,v->name)){
		#ifndef BENCH
		printf("cadastro com Nome \"%s\" duplicado, porém adicionando no bd\n",(*a)->name);
		#endif
		basenodestack_push(&(*a)->s,basenode_copy(v));
		return 2;
	}
	else if(x>y){
		(*a)->fb++;
		flag=avl_insert(&(*a)->r,v,flag);
	}
	else{
		(*a)->fb--;
		flag=avl_insert(&(*a)->l,v,flag);
	}
	if(flag==2)return 2;
	if((*a)->fb > 1){
		flag=1;
		spn=strspn(v->name,(*a)->r->name);
		x=v      ->name[spn];
		y=(*a)->r->name[spn];
		if(x > y){
			avl_rotleft(a);
		}
		else{
			avl_rotleftright(a);
		}
		return flag;
	}
	else if((*a)->fb < -1){
		flag=1;
		spn=strspn(v->name,(*a)->l->name);
		x=v      ->name[spn];
		y=(*a)->l->name[spn];
		if(x < y){
			avl_rotright(a);
		}
		else{
			avl_rotrightleft(a);
		}
		return flag;
	}
	if(flag){
		//força bruta!!
		(*a)->fb=(avl_height((*a)->r)-avl_height((*a)->l));
		//atualizar fatores dos filhos, se tiver
		if((*a)->r)
			(*a)->r->fb=(avl_height((*a)->r->r)-avl_height((*a)->r->l));
		if((*a)->l)
			(*a)->l->fb=(avl_height((*a)->l->r)-avl_height((*a)->l->l));
		// avl_rebalance(*a);
	}
	return flag;
}
int avl_insert2(AvlNode** a,AvlNode* v,int flag){
	if(!*a){
		*a=v;
		return 0;
	}
	uint spn;
	char x,y;
	spn=strspn(v->name,(*a)->name);
	x=v   ->name[spn];
	y=(*a)->name[spn];
	if(!strcmp((*a)->name,v->name)){
		err("comportamento indefinido:não pode inserir nó com duplicados em um nó duplicado\n");
	}
	else if(x>y){
		(*a)->fb++;
		flag=avl_insert2(&(*a)->r,v,flag);
	}
	else{
		(*a)->fb--;
		flag=avl_insert2(&(*a)->l,v,flag);
	}
	if(flag==2)return 2;
	if((*a)->fb > 1){
		flag=1;
		spn=strspn(v->name,(*a)->r->name);
		x=v      ->name[spn];
		y=(*a)->r->name[spn];
		if(x > y){
			avl_rotleft(a);
		}
		else{
			avl_rotleftright(a);
		}
		return flag;
	}
	else if((*a)->fb < -1){
		flag=1;
		spn=strspn(v->name,(*a)->l->name);
		x=v      ->name[spn];
		y=(*a)->l->name[spn];
		if(x < y){
			avl_rotright(a);
		}
		else{
			avl_rotrightleft(a);
		}
		return flag;
	}
	if(flag){
		//força bruta!!
		(*a)->fb=(avl_height((*a)->r)-avl_height((*a)->l));
		//atualizar fatores dos filhos, se tiver
		if((*a)->r)
			(*a)->r->fb=(avl_height((*a)->r->r)-avl_height((*a)->r->l));
		if((*a)->l)
			(*a)->l->fb=(avl_height((*a)->l->r)-avl_height((*a)->l->l));
		// avl_rebalance(*a);
	}
	return flag;
}
AvlNode* avl_remove(AvlNode**a,char* str,char* flag){
	if(!*a){
		*flag=2;
		return 0;
	}
	AvlNode* ans;
	uint spn;
	char x,y;
	spn=strspn(str,(*a)->name);
	x=str       [spn];
	y=(*a)->name[spn];
	if(!strcmp(str,(*a)->name)){		
		ans=*a;
		if((*a)->l&&(*a)->r){
			AvlNode* aux=(*a)->l;
			while(aux->r){
				aux=aux->r;
			}
			AvlNode*rans=avl_newNode(NULL);
			avl_nodecopy(rans,*a);
			rans->s=(*a)->s;
			(*a)->s=0;
			avl_nodecopy(*a,aux);
			char f=0;
			ans=avl_remove(&(*a)->l,aux->name,&f);
			free(ans);
			ans=rans;
		}
		else if((*a)->r){
			*a=(*a)->r;
		}
		else if((*a)->l){
			*a=(*a)->l;
		}
		else{
			*a=NULL;
		}
		return ans;
	}
	else if(x>y){
		(*a)->fb--;
		ans=avl_remove(&(*a)->r,str,flag);
	}
	else{
		(*a)->fb++;
		ans=avl_remove(&(*a)->l,str,flag);
	}
	if(*flag==2){//se n encontrar no, disfaça alteraçoes em fb
		if(x>y){
			(*a)->fb++;
		}
		else{
			(*a)->fb--;
		}
		return ans;
	}
	if((*a)->fb>1){
		spn=strspn(str,(*a)->r->name);
		x=str[spn];
		y=(*a)->r->name[spn];
		*flag=1;
		if(x>y){
			avl_rotleft(a);
		}
		else{
			avl_rotleftright(a);
		}
		return ans;
	}
	else if((*a)->fb<-1){
		spn=strspn(str,(*a)->r->name);
		x=str[spn];
		y=(*a)->r->name[spn];
		*flag=1;
		if(x < y){
			avl_rotright(a);
		}
		else{
			avl_rotrightleft(a);
		}
		return ans;
	}
	if(*flag==1){
		//força bruta!!
		(*a)->fb=(avl_height((*a)->r)-avl_height((*a)->l));
		//atualizar fatores dos filhos, se tiver
		if((*a)->r)
			(*a)->r->fb=(avl_height((*a)->r->r)-avl_height((*a)->r->l));
		if((*a)->l)
			(*a)->l->fb=(avl_height((*a)->l->r)-avl_height((*a)->l->l));
		// avl_rebalance(*a);
	}
	return ans;

}
int avl_printCompatibleDonors(AvlNode* t,BloodType bt){
	if(!t)return 0;
	int ans=0;
	if(t->s){
		BaseNodeStack*s=t->s;
		while(s){
			if(bloodtype_iscompatible(bt,t->bt)){
				basenode_print(s->v);
				ans=1;
			}
			s=s->n;
		}
	}
	if(bloodtype_iscompatible(bt,t->bt)){
		avl_printNode(t);
		ans=1;
	}
	if(avl_printCompatibleDonors(t->l,bt))ans=1;
	if(avl_printCompatibleDonors(t->r,bt))ans=1;
	return ans;
}
int avl_printBloodType(AvlNode*t,BloodType bt){
	if(!t)return 0;
	int ans=0;
	if(t->s){
		BaseNodeStack*s=t->s;
		while(s){
			if(bt==s->v->bt){
				basenode_print(s->v);
				ans=1;
			}
			s=s->n;
		}
	}
	if(bt==t->bt){
		avl_printNode(t);
		ans=1;
	}
	if(avl_printBloodType(t->l,bt))ans=1;
	if(avl_printBloodType(t->r,bt))ans=1;
	return ans;
}
void avl_printNode(AvlNode*t){
	#ifndef BENCH //se for benchmark, nao imprima nada
	if(!t){
		printf("Cadastro não encontrado\n");
		return;
	}
	char blood[5];
	bloodTypeToString(t->bt,blood);
	printf("Nome:%s\n",t->name);
	printf("Idade:%u\n",t->age);
	printf("Tipo de sangue:%s\n",blood);
	printf("Sangue doado:%.2f mL\n",t->dntbld);
	printf("------------------\n");
	#endif
}
void avl_print(AvlNode*t){
	if(!t)return;
	avl_print(t->l);
	if(t->s){
		BaseNodeStack*s=t->s;
		while(s){
			basenode_print(s->v);
			s=s->n;
		}
	}
	avl_printNode(t);
	avl_print(t->r);
}