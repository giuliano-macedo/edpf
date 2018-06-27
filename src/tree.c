#include "tree.h"
TreeNode* tree_new(){
	return NULL;
}
TreeNode* tree_newNode(BaseNode*n){
	TreeNode* ans=(TreeNode*)calloc(1,sizeof(TreeNode));
	strcpy(ans->name,n->name);
	ans->age=n->age;
	ans->bt=n->bt;
	ans->dntbld=n->dntbld;
	return ans;
}
void tree_nodecopy(TreeNode*x,TreeNode*y){
	strcpy(x->name,y->name);
	x->age=y->age;
	x->bt=y->bt;
	x->dntbld=y->dntbld;
}
int tree_printAge(TreeNode*t,uint age){
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
		tree_printNode(t);
		ans=1;
	}
	if(tree_printAge(t->l,age))ans=1;
	if(tree_printAge(t->r,age))ans=1;
	return ans;
}
TreeNode** tree_searchByName(TreeNode**t,char* str){
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
		return t;
	}
	if((str[spn]>(*t)->name[spn])){
		return tree_searchByName(&(*t)->r,str);
	}
	return tree_searchByName(&(*t)->l,str);
}
void tree_getGalon(TreeNode* t,float* g){
	if(!t) return;
	if(t->s){
		BaseNodeStack*s=t->s;
		while(s){
			g[s->v->bt]+=s->v->dntbld;
			s=s->n;
		}
	}
	g[t->bt]+=t->dntbld;
	tree_getGalon(t->r,g);
	tree_getGalon(t->l,g);
}
void tree_removeNode(TreeNode**t){
	TreeNode*ans=*t;
	if((*t)->l&&(*t)->r){
		TreeNode* aux=(*t)->l;
		while(aux->r){
			aux=aux->r;
		}
		tree_nodecopy(*t,aux);
		tree_removeNode(tree_searchByName(&(*t)->l,aux->name));
		return ;
	}
	else if((*t)->r){
		*t=(*t)->r;
	}
	else if((*t)->l){
		*t=(*t)->l;
	}
	else{
		printf("FOLHA\n");
		*t=NULL;
	}
	free(ans);
}
void tree_insert(TreeNode** t,BaseNode* n){
	if(!*t){
		*t=tree_newNode(n);
		return;
	}
	uint spn=strspn(n->name,(*t)->name);
	if(!strcmp((*t)->name,n->name)){
		#ifndef BENCH
		printf("cadastro com Nome \"%s\" duplicado, porém adicionando no bd\n",(*t)->name);
		#endif
		basenodestack_push(&(*t)->s,basenode_copy(n));
	}
	else if((n->name[spn]>(*t)->name[spn])){
		tree_insert(&(*t)->r,n);
	}
	else{
		tree_insert(&(*t)->l,n);
	}
}
void tree_insert2(TreeNode** t,TreeNode* n){
	if(!*t){
		*t=n;
		return;
	}
	uint spn=strspn(n->name,(*t)->name);
	if(!strcmp((*t)->name,n->name)){
		err("comportamento indefinido:não pode inserir nó com duplicados em um nó duplicado\n");
	}
	else if((n->name[spn]>(*t)->name[spn])){
		tree_insert2(&(*t)->r,n);
	}
	else{
		tree_insert2(&(*t)->l,n);
	}
}
int tree_printCompatibleDonors(TreeNode* t,BloodType bt){
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
		tree_printNode(t);
		ans=1;
	}
	if(tree_printCompatibleDonors(t->l,bt))ans=1;
	if(tree_printCompatibleDonors(t->r,bt))ans=1;
	return ans;
}
int tree_printBloodType(TreeNode*t,BloodType bt){
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
		tree_printNode(t);
		ans=1;
	}
	if(tree_printBloodType(t->l,bt))ans=1;
	if(tree_printBloodType(t->r,bt))ans=1;
	return ans;
}
void tree_printNode(TreeNode*t){
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
void tree_print(TreeNode*t){
	if(!t)return;
	tree_print(t->l);
	if(t->s){
		BaseNodeStack*s=t->s;
		while(s){
			basenode_print(s->v);
			s=s->n;
		}
	}
	tree_printNode(t);
	tree_print(t->r);
}