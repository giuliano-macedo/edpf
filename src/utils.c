#include "bbmed.h"
void err(char* msg){
	fprintf(stderr, "%s\n", msg);
	exit(-1);
}
void basenode_print(BaseNode* n){
    if(!n)return;
    #ifndef BENCH
    char blood[5];
    bloodTypeToString(n->bt,blood);
    printf("Nome:%s\n",n->name);
    printf("Idade:%u\n",n->age);
    printf("Tipo de sangue:%s\n",blood);
    printf("Sangue doado:%.2f mL\n",n->dntbld);
    printf("------------------\n");
    #endif
}
BaseNode* basenode_copy(BaseNode* n){
    BaseNode* ans=(BaseNode*)malloc(sizeof(BaseNode));
    strcpy(ans->name,n->name);
    ans->age=n->age;
    ans->bt=n->bt;
    ans->dntbld=n->dntbld;
    return ans;
}
int bloodtype_iscompatible(BloodType x,BloodType y){
    if(y==oneg||x==abpos)return 1;
    switch(x){
        case apos:
            return !(y==abpos||y==abneg);//a+ só não doa para ab+ e ab-
        case opos:
            return (y==opos);//o+ só recebe de o
        case bpos:
            return (y==bpos||y==bneg||y==opos);//b+ so recebe de b e o
        case aneg:
            return (y==aneg);//a- so recebe de a- e o-
        case bneg:
            return (y==bneg);//b- so recebe de b- e o-
        case abneg:
            return (y==abneg||y==aneg||y==bneg);//ab- só recebe de negativo
    }
    return 0;
}
void basenodestack_push(BaseNodeStack** s,BaseNode* v){
    BaseNodeStack *n=(BaseNodeStack*)calloc(1,sizeof(BaseNodeStack));
    n->v=v;
    if(!*s)*s=n;
    else{
        n->n=*s;
        *s=n;
    }
}
BaseNode* basenodestack_pop (BaseNodeStack** s){
    if(!*s)return 0;
    BaseNodeStack*temp = *s;
    *s=(*s)->n;
    BaseNode*ans=temp->v;
    free(temp);
    return ans;
}
void galon_print(float*gal){
    printf("----------------------------\n");
    printf("Quantidade de sangue doada para cada tipo em mL:\n");
    printf("A+ :%.2f\n",gal[1]);
    printf("B+ :%.2f\n",gal[2]);
    printf("AB+:%.2f\n",gal[3]);
    printf("O+ :%.2f\n",gal[4]);
    printf("A- :%.2f\n",gal[5]);
    printf("B- :%.2f\n",gal[6]);
    printf("AB-:%.2f\n",gal[7]);
    printf("O- :%.2f\n",gal[8]);
    printf("Somente positivo:%.2f\n",gal[1]+gal[2]+gal[3]+gal[4]);
    printf("Somente negativo:%.2f\n",gal[5]+gal[6]+gal[7]+gal[8]);
    printf("Somente A :%.2f\n",gal[1]+gal[5]);
    printf("Somente B :%.2f\n",gal[2]+gal[6]);
    printf("Somente AB:%.2f\n",gal[3]+gal[7]);
    printf("Somente O :%.2f\n",gal[4]+gal[8]);
    printf("----------------------------\n");
}
BloodType parseType(char* str){
	BloodType ans=nullbloodtype;
	char c;
	uint i=0;
	while((c=str[i]))str[i++]=tolower(c);
	if(!strcmp(str,"a+"))ans=apos;
	else if(!strcmp(str,"a-"))ans=aneg;
	else if(!strcmp(str,"b+"))ans=bpos;
	else if(!strcmp(str,"b-"))ans=bneg;
	else if(!strcmp(str,"ab+"))ans=abpos;
	else if(!strcmp(str,"ab-"))ans=abneg;
	else if(!strcmp(str,"o+"))ans=opos;
	else if(!strcmp(str,"o-"))ans=oneg;
	return ans;
}
void bloodTypeToString(BloodType bt,char* ans){
    strcpy(ans,"Null");
    if(bt==apos)strcpy(ans,"A+");
    else if(bt==aneg)strcpy(ans,"A-");
    else if(bt==bpos)strcpy(ans,"B+");
    else if(bt==bneg)strcpy(ans,"B-");
    else if(bt==abpos)strcpy(ans,"AB+");
    else if(bt==abneg)strcpy(ans,"AB-");
    else if(bt==opos)strcpy(ans,"O+");
    else if(bt==oneg)strcpy(ans,"O-");
}
uint countValidLines(FILE* f){
	BaseNode temp;
	char buff[256];
	char tempstr[3];
	int exc;
	uint ans=0;
	while((fgets(buff,256,f))){
    	int res=sscanf(buff,"%30[^,],%i,%3[^,],%f%n",
		temp.name,&temp.age,tempstr,&temp.dntbld,&exc);
		temp.bt=parseType(tempstr);
		if(temp.bt&&res==4&&(buff[exc]=='\n'||!buff[exc]))ans++;
	}
	fseek(f,0,SEEK_SET);
	return ans;
}
void bd_init(DataBase*d,DataBaseType dt){
    BDSWITCH(dt,
                    d->t=tree_new();
                    ,
                    err("narvore não implementada ainda :(\n");
                    d->nt=ntree_new();
                    ,
                    d->a=avl_new();
                    ,
                    err("navl não implementada ainda :(\n");
                    d->na=navl_new();
                    ,
                    d->h=hashtable_new();
                )
}
uint bd_read(DataBase* b,FILE* bdfile,DataBaseType bt){
    uint n=countValidLines(bdfile);
    if(bt==hashtable)hashtable_setSize(b->h,n);
    int l=1;
    BaseNode temp;
    char tempstr[4];
    char buff[256];
    int exc;
    int res;
    int cond;
    while((fgets(buff,256,bdfile))){
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
            BDSWITCH(bt,
            	tree_insert(&b->t,&temp);
            	,
            	ntree_insert(b->nt,&temp);
            	,
				avl_insert(&b->a,&temp,0);
				,
				navl_insert(&b->na,&temp);
				,
				hashtable_insert(b->h,&temp);
            )
        }
        else{
            printf("linha %i do arquivo tem um formato incorreto e foi ignorada\n",l);
        }
        l++;
    }
    fclose(bdfile);
    return n;
}