#include "benchmark.h"
#include "bbmed.h"
long unsigned int timer(struct timespec* s,struct timespec* e,int f){
	if(f){
		clock_gettime(CLOCK_MONOTONIC_RAW, s);
		return 0;  
	}
	clock_gettime(CLOCK_MONOTONIC_RAW, e);
	return (e->tv_sec - s->tv_sec) * 1000000 + (e->tv_nsec - s->tv_nsec) / 1000;
}
int main(int argc,char** argv){
	if(argc!=2){
		printf("[USO] %s [arquivo de usuarios]\n",argv[0]);
		return 2;
	}
	FILE* f=fopen(argv[1],"r");
	if(!f)err("Erro ao abrir arquivo de usuário\n");
	uint n=countValidLines(f);
	BaseNodeStack* toRemove=0;
	uint noit=n/3;
	TreeNode*  t=tree_new();
	// AvlNode*   a=avl_new();
	HashTable* h=hashtable_new();

	long unsigned int ttime=0,atime=0,htime=0;
	hashtable_setSize(h,n);
	struct timespec s,e;
	uint l=0;
	char buff[256];
	BaseNode temp;
	char tempstr[32];
	int cond,res,exc;
	  while((fgets(buff,256,f))){
        if(strlen(buff)==255){
            printf("Erro, a linha de um arquivo nao pode ultrapassar 255 caracteres!\n");
            exit(-1);
        }
        if(l!=n){
        	res=sscanf(buff,"%30[^,],%i,%4[^,],%f\n%n",
        		temp.name,&temp.age,tempstr,&temp.dntbld,&exc);
            temp.bt=parseType(tempstr);
            cond=(temp.bt&&res==4&&!buff[exc]);
        }
        else{
        	res=sscanf(buff,"%30[^,],%i,%4[^,],%f%n",
        		temp.name,&temp.age,tempstr,&temp.dntbld,&exc);
            temp.bt=parseType(tempstr);
            cond=(temp.bt&&res==4&&(buff[exc]=='\n'||!buff[exc]));
        }
        if(cond){
        	if(l<=noit){
        		basenodestack_push(&toRemove,basenode_copy(&temp));
        	}
        	timer(&s,&e,1);
        	tree_insert(&t,&temp);
			ttime+=timer(&s,&e,0);
			
			timer(&s,&e,1);
        	// avl_insert(&a,&temp,0);
			atime+=timer(&s,&e,0);
			
			timer(&s,&e,1);
			hashtable_insert(h,&temp);
			htime+=timer(&s,&e,0);
        }
        
        else{
            printf("linha %i do arquivo tem um formato incorreto e foi ignorada\n",l);
        }
        l++;
    }
    printf("no de colisoes tabela hash: %u\n",h->noColisions);
    printf("arvore , avl , tabelahash média em microsegundos\n");
    printf("insere : %lf , %lf , %lf\n",((double)ttime/n),((double)atime/n),((double)htime/n));
    ttime=atime=htime=0;

}