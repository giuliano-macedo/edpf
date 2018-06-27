#include <stdio.h>
#include <stdlib.h>
typedef struct AvlNode {
	int v;
	struct AvlNode* l;
	struct AvlNode* r;
	char fb;
}AvlNode;
AvlNode* avl_newNode(int v){
	AvlNode* ans=(AvlNode*)calloc(1,sizeof(AvlNode));
	ans->v=v;
	return ans;
}
void avl_switchNode(AvlNode**x,AvlNode**y){
	AvlNode*temp;
	temp=*x;
	*x=*y;
	*y=temp;
}
int avl_height(AvlNode*a){
	if(!a)return 0;
	int hl=avl_height(a->l)+1;
	int hr=avl_height(a->r)+1;
	if(hl>hr)return hl;
	return hr;
}
void avl_printIfHeight(AvlNode*a,int h){
	if(!a)return;
	if(avl_height(a)==h)printf("(%i,%i) ",a->v,a->fb);
	avl_printIfHeight(a->l,h);
	avl_printIfHeight(a->r,h);
}
void avl_printLevel(AvlNode*a){
	int h=avl_height(a);
	for(int i=h;i>0;i--){
		for(int j=0;j<i;j++)printf(" ");
		avl_printIfHeight(a,i);
		putchar('\n');
	}
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
void avl_rebalance(AvlNode*a){
	if(!a)return;
	a->fb=(avl_height(a->r)-avl_height(a->l));//força bruta!!
	avl_rebalance(a->l);
	avl_rebalance(a->r);
}
int avl_insert(AvlNode**a,int v,char flag){
	if(!*a){
		*a=avl_newNode(v);
		return 0;
	}
	else if(v>(*a)->v){
		(*a)->fb++;
		flag=avl_insert(&(*a)->r,v,flag);
	}
	else{
		(*a)->fb--;
		flag=avl_insert(&(*a)->l,v,flag);
	}
	if((*a)->fb>1){
		flag=1;
		if(v > (*a)->r->v){
			avl_rotleft(a);
		}
		else{
			avl_rotleftright(a);
		}
		return flag;
	}
	else if((*a)->fb<-1){
		flag=1;
		if(v < (*a)->l->v){
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
AvlNode* avl_remove(AvlNode**a,int v,int* flag){
	if(!*a) return 0;
	AvlNode* ans;
	if((*a)->v==v){		
		ans=*a;
		if((*a)->l&&(*a)->r){
			AvlNode* aux=(*a)->l;
			while(aux->r){
				aux=aux->r;
			}
			// tree_nodecopy(*a,aux);
			(*a)->v=aux->v;
			int f=0;
			ans=avl_remove(&(*a)->l,aux->v,&f);
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
	else if(v>(*a)->v){
		(*a)->fb--;
		ans=avl_remove(&(*a)->r,v,flag);
	}
	else{
		(*a)->fb++;
		ans=avl_remove(&(*a)->l,v,flag);
	}
	if((*a)->fb>1){
		*flag=1;
		if(v > (*a)->r->v){
			printf("LEFT ROTATE\n");
			avl_rotleft(a);
		}
		else{
			printf("LEFT RIGHT ROTATE\n");
			avl_rotleftright(a);
		}
		return ans;
	}
	else if((*a)->fb<-1){
		*flag=1;
		if(v < (*a)->l->v){
			printf("RIGHT ROTATE\n");
			avl_rotright(a);
		}
		else{
			printf("RIGHT LEFT ROTATE\n");
			avl_rotrightleft(a);
		}
		return ans;
	}
	if(*flag){
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
int main(){
	AvlNode* a=NULL;
	avl_insert(&a,2,0);
	avl_insert(&a,7,0);
	avl_insert(&a,10,0);
	avl_insert(&a,4,0);
	avl_insert(&a,6,0);
	avl_insert(&a,11,0);
	avl_insert(&a,1,0);
	avl_insert(&a,8,0);
	avl_insert(&a,13,0);
	avl_insert(&a,14,0);
	avl_insert(&a,9,0);
	avl_insert(&a,12,0);
	avl_insert(&a,3,0);
	avl_insert(&a,5,0);
	avl_insert(&a,15,0);

	AvlNode* rn;
	int flag;
	flag=0;
	avl_printLevel(a);
	rn=avl_remove(&a,13,&flag);
	avl_printLevel(a);
	printf("node %i l:",rn->v);
	if(rn->l)printf("%i",rn->l->v);
	else printf("null");
	printf(" r:");
	if(rn->r)printf("%i",rn->r->v);
	else printf("null");
	printf("\n");
	flag=0;
	avl_remove(&a,2,&flag);
	// flag=0;
	// avl_remove(&a,8,&flag);
	// flag=0;
	// avl_remove(&a,3,&flag);

	avl_printLevel(a);

	// avl_rebalance(a);

	// avl_printLevel(a);
	// avl_insert(&a,'m');
	// avl_insert(&a,'n');
	// avl_insert(&a,'o');
	// avl_insert(&a,'l');
	// avl_insert(&a,'k');
	// avl_insert(&a,'q');
	// avl_insert(&a,'p');
	// avl_insert(&a,'h');
	// avl_insert(&a,'i');
	// avl_insert(&a,'a');
	// avl_print(a);
}