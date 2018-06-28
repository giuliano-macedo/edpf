#include "bbmed.h"
void help(char**argv){
	printf("[USO] %s [opcao de bd] [arquivo de usuarios]\n",argv[0]);
	printf("as opcoes de bd podem ser:\n");
	printf("arvore : cria 1 arvore chaveada apenas para nome\n");
	printf("narvore : cria 4 arvores todas chaveadas para cada campo\n");
	printf("avl : igual a arvore porem balanceada\n");
	printf("navl : igual a narvore porem balanceada\n");
	printf("tabelahash : cria uma tabela hash chaveada no nome\n");
}
void readString(char*ans){
	scanf("%[^\n]s",ans);
	getchar();
}
void readChar(char* c,char* buff){
	readString(buff);
	*c=buff[0];
}
void readUint(uint* ui,char* buff){
	readString(buff);
	int ans=atoi(buff);
	if(ans<0)*ui=0;
	else *ui=(uint)ans;
}
void readFloat(float* f,char*buff){
	readString(buff);
	*f=(float)atof(buff);
}
DataBaseType parseBdOptions(char* str){
	DataBaseType ans=nullbloodtype;
	if(!strcmp(str,"arvore"))ans=tree;
	else if(!strcmp(str,"narvore"))ans=ntree;
	else if(!strcmp(str,"avl"))ans=avl;
	else if(!strcmp(str,"navl"))ans=navl;
	else if(!strcmp(str,"tabelahash"))ans=hashtable;
	return ans;
}
int main(int argc,char** argv){
	if(argc!=3){
		help(argv);
		return 2;
	}
	DataBase bd;
	DataBaseType bdtype=parseBdOptions(argv[1]);
	if(!bdtype){
		printf("Comando invalido (%s)\n",argv[1]);
		help(argv);
		return -1;
	}
	FILE* f=fopen(argv[2],"r");
	if(!f)err("Erro ao abrir arquivo de usuarios");
	bd_init(&bd,bdtype);
	uint n=bd_read(&bd,f,bdtype);
	printf("O banco de dados tem %u usuarios cadastrados\n",n);
	TreeNode** t;
	// NtreeNode** nt;
	AvlNode* a;
	BaseNode* bn;
	// NavlNode** na;
	BaseNode temp;
	BloodType bt;
	
	float galon[10];
	char str[256];
	char flag;
	while(1){
		char c;
		printf("Selecione uma das opções\n");
		printf("[A]tualizar\n");
		printf("[B]uscar\n");
		printf("[C]adastrar\n");
		printf("[L]istar\n");
		printf("[R]emover\n");
		printf("[S]air\n");
		readChar(&c,str);
		switch(c){
			case 'b':
			case 'B':
				printf("Digite o nome da pessoa ser pesquisada\n");
				readString(str);
				temp.bt=nullbloodtype;
				BDSWITCH(bdtype,
	            	t=tree_searchByName(&bd.t,str);
	            	if(t){
	            		strcpy(temp.name,(*t)->name);
	            		temp.age=(*t)->age;
	            		temp.bt=(*t)->bt;
	            		temp.dntbld=(*t)->dntbld;
	            	}
	            	,
	            	//ntree..
	            	,
					a=avl_searchByName(&bd.a,str);
					if(a){
						strcpy(temp.name,a->name);
	            		temp.age=a->age;
	            		temp.bt=a->bt;
	            		temp.dntbld=a->dntbld;
					}
					,
					//navl..
					,
					bn=hashtable_searchByName(bd.h,str);
					if(bn){
						strcpy(temp.name,bn->name);
						temp.age=bn->age;
	            		temp.bt=bn->bt;
	            		temp.dntbld=bn->dntbld;
					}
            	)
            	if(!temp.bt)printf("Usuário não encontrado\n");
            	else{
            		basenode_print(&temp);
            	}
				break;
			case 'c':
			case 'C':
				printf("Digite o nome do paciente:\n");
				readString(str);
				strcpy(temp.name,str);
				printf("Digite a idade do paciente:\n");
				readUint(&temp.age,str);
				printf("Digite o tipo sanguineo do paciente:\n");
				readString(str);
				temp.bt=parseType(str);
				if(!temp.bt){
					printf("Tipo sanguineo invalido\n");
					break;
				}
				printf("Digite a quantidade de sangue doada (em mL):\n");
				readFloat(&temp.dntbld,str);
				BDSWITCH(bdtype,	
	            	tree_insert(&bd.t,&temp);
	            	,
	            	//ntree..
	            	,
					avl_insert(&bd.a,&temp,0);
					,
					//navl..
					,
					hashtable_insert(bd.h,&temp);
            	)
				break;
			case 'r':
			case 'R':
				printf("Digite o nome da pessoa ser removida\n");
				readString(str);
				BDSWITCH(bdtype,
	            	t=tree_searchByName(&bd.t,str);
	            	if(t==NULL){
	            		printf("Usuário não encontrado\n");
	            	}
	            	else{
	            		printf("Usuário a seguir foi removido\n");
	            		tree_printNode(*t);
	            		if((*t)->s){
	            			printf("Cadastro duplicado encontrado!\n");
	            			printf("O resto dos cadastros duplicados foram preservados\n");
	            			BaseNode* aux=basenodestack_pop(&(*t)->s);
	            			TreeNode* tempn=tree_newNode(aux);
	            			tempn->s=(*t)->s;
	            			tree_removeNode(t);
	            			tree_insert2(&bd.t,tempn);
	            			free(aux);
	            		}
	            		else{
	            			tree_removeNode(t);
	            		}
	            	}
	            	,
	            	//ntree..
	            	,
	            	flag=0;
					a=avl_remove(&bd.a,str,&flag);
					if(!a){
						printf("Usuário não encontrado\n");
						break;
					}
					printf("Usuário a seguir foi removido\n");
					avl_printNode(a);
					if(a->s){
						printf("Cadastro duplicado encontrado!\n");
            			printf("O resto dos cadastros duplicados foram preservados\n");
            			BaseNode* aux=basenodestack_pop(&a->s);
            			AvlNode* tempn=avl_newNode(aux);
            			tempn->s=a->s;
            			avl_insert2(&bd.a,tempn,0);
            			free(aux);
					}
					,
					//navl..
					,
					bn=hashtable_remove(bd.h,str);
					if(!bn){
						printf("usuário não encontrado\n");
						break;
					}
					printf("Usuário a seguir foi removido\n");
					basenode_print(bn);
					if(hashtable_searchByName(bd.h,str)){
						printf("Cadastro duplicado encontrado\n");
						printf("O resto dos cadastros duplicados foram preservados\n");
					}
            	)
				break;
			case 'a':
			case 'A':
				printf("Digite o nome da pessoa ser atualizada\n");
				readString(str);
				temp.bt=nullbloodtype;
				BDSWITCH(bdtype,
	            	t=tree_searchByName(&bd.t,str);
	            	if(t==NULL)break;
	            	else{
	            		strcpy(temp.name,(*t)->name);
	            		temp.age=(*t)->age;
	            		temp.bt=(*t)->bt;
	            		temp.dntbld=(*t)->dntbld;
	            		if((*t)->s){
	            			printf("O resto dos cadastros duplicados foram preservados\n");
	            			BaseNode* aux=basenodestack_pop(&(*t)->s);
	            			TreeNode* tempn=tree_newNode(aux);
	            			tempn->s=(*t)->s;
	            			tree_removeNode(t);
	            			tree_insert2(&bd.t,tempn);
	            			free(aux);
	            		}
	            		else{
	            			tree_removeNode(t);
	            		}
	            	}
	            	,
	            	//ntree..
	            	,
					flag=0;
					a=avl_remove(&bd.a,str,&flag);
					if(!a){
						break;
					}
					strcpy(temp.name,a->name);
					temp.age=a->age;
            		temp.bt=a->bt;
            		temp.dntbld=a->dntbld;
					if(a->s){
						printf("Cadastro duplicado encontrado!\n");
            			printf("O resto dos cadastros duplicados foram preservados\n");
            			BaseNode* aux=basenodestack_pop(&a->s);
            			AvlNode* tempn=avl_newNode(aux);
            			tempn->s=a->s;
            			avl_insert2(&bd.a,tempn,0);
            			free(aux);
					}
					,
					//navl..
					,
					bn=hashtable_remove(bd.h,str);
					if(!bn){
						break;
					}
					strcpy(temp.name,bn->name);
					temp.age=bn->age;
            		temp.bt=bn->bt;
            		temp.dntbld=bn->dntbld;
					if(hashtable_searchByName(bd.h,str)){
						printf("Cadastro duplicado encontrado\n");
						printf("O resto dos cadastros duplicados foram preservados\n");
					}
            	)
            	if(!temp.bt){
            		printf("Nome não encontrado\n");
            		break;
            	}
    		    printf("Nome atual:%s\n",temp.name);
        		printf("Deseja mudar?\n");
        		printf("[S]im\n");
        		printf("[N]ao\n");
        		readChar(&c,str);
        		c=tolower(c);
        		if(c=='s'){
        			printf("Digite o novo nome:\n");
        			readString(str);
        			strcpy(temp.name,str);
        		}
        		printf("idade atual:%i\n",temp.age);
        		printf("Deseja mudar?\n");
        		printf("[S]im\n");
        		printf("[N]ao\n");
        		readChar(&c,str);
        		c=tolower(c);
        		if(c=='s'){
        			printf("Digite a nova idade:\n");
        			readUint(&temp.age,str);
        		}
        		bloodTypeToString(temp.bt,str);
        		printf("tipo sanguineo atual:%s\n",str);
        		printf("Deseja mudar?\n");
        		printf("[S]im\n");
        		printf("[N]ao\n");
        		readChar(&c,str);
        		c=tolower(c);
        		if(c=='s'){
        			printf("Digite o novo tipo sanguineo:\n");
	        		readString(str);
	        		bt=parseType(str);
	        		if(!bt){
	        			printf("Tipo sanguineo invalido, deixando inalterado\n");
	        		}
	        		else{
	        			temp.bt=bt;
	        		}
        		}
        		printf("quantidade de sangue doada atual:%f\n",temp.dntbld);
        		printf("Deseja mudar?\n");
        		printf("[S]im\n");
        		printf("[N]ao\n");
        		readChar(&c,str);
        		c=tolower(c);
        		if(c=='s'){
        			printf("Digite a nova quantidade de sangue doada:\n");
        			readFloat(&temp.dntbld,str);
        		}
        		BDSWITCH(bdtype,	
	            	tree_insert(&bd.t,&temp);
	            	,
	            	//ntree..
	            	,
					avl_insert(&bd.a,&temp,0);
					,
					//navl..
					,
					hashtable_insert(bd.h,&temp);
            	)
				break;
			case 'l':
			case 'L':
				printf("Selecione uma das opções\n");
				printf("[L]istar todos usuários\n");
				printf("listar todos usuários com     [I]dades iguais\n");
				printf("listar todo usuários com tipo [S]anguineo igual\n");
				printf("listar doadores [C]ompatíveis\n");
				printf("listar [Q]uantidade de sangue de cada tipo\n");
				readChar(&c,str);
				switch(c){
					case 'S':
					case 's':
						printf("Digite o tipo de sangue a ser pesquisado:\n");
						readString(str);
						bt=parseType(str);
						if(!bt){
							printf("Tipo de sangue inválido\n");
							break;
						}
						BDSWITCH(bdtype,
			            	if(!tree_printBloodType(bd.t,bt)){
			            		printf("Nenhum usuário encontrado\n");
			            	}
			            	,
			            	//ntree..
			            	,
							if(!avl_printBloodType(bd.a,bt)){
								printf("Nenhum usuário encontrado\n");
							}
							,
							//navl..
							,
							if(!hashtable_printBloodType(bd.h,bt)){
								printf("Nenhum usuário encontrado\n");
							}
		            	)
		            	break;
					case 'l':
					case 'L':
						BDSWITCH(bdtype,
			            	tree_print(bd.t);
			            	,
			            	//ntree..
			            	,
							avl_print(bd.a);
							,
							//navl..
							,
							hashtable_print(bd.h);
		            	)
						break;
					case 'i':
					case 'I':
						printf("Digite a idade:\n");
						readUint(&temp.age,str);
						BDSWITCH(bdtype,
			            	if(!tree_printAge(bd.t,temp.age)){
			            		printf("Nenhum usuário encontrado\n");
			            	}
			            	,
			            	//ntree..
			            	,
							if(!avl_printAge(bd.a,temp.age)){
			            		printf("Nenhum usuário encontrado\n");
			            	}
							,
							//navl..
							,
							if(!hashtable_printAge(bd.h,temp.age)){
			            		printf("Nenhum usuário encontrado\n");
			            	}
		            	)
						break;
					case 'c':
					case 'C':
						printf("Digite o tipo de sangue do recipiente:\n");
						readString(str);
						bt=parseType(str);
						if(!bt){
							printf("Tipo de sangue inválido\n");
							break;
						}
						BDSWITCH(bdtype,
			            	if(!tree_printCompatibleDonors(bd.t,bt)){
			            		printf("Nenhum usuário encontrado\n");
			            	}
			            	,
			            	//ntree..
			            	,
							if(!avl_printCompatibleDonors(bd.a,bt)){
			            		printf("Nenhum usuário encontrado\n");
			            	}
							,
							//navl..
							,
							if(!hashtable_printCompatibleDonors(bd.h,bt)){
			            		printf("Nenhum usuário encontrado\n");
			            	}
		            	)
						break;
					case 'q':
					case 'Q':
						for(uint i=0;i<9;i++){
							galon[i]=0.0;
						}
						BDSWITCH(bdtype,
					            	tree_getGalon(bd.t,galon);
					            	,
					            	//ntree..
					            	,
									avl_getGalon(bd.a,galon);
									,
									//navl..
									,
									hashtable_getGalon(bd.h,galon);
				            	)
					    galon_print(galon);
						break;
				}
				break;
			case 's':
			case 'S':
				return 0;
				break;
		}
		printf("Aperte enter para continuar\n");
		getchar();
		#ifdef _WIN32
			system("cls");
		#endif
		#ifdef __linux__
			// system("clear");
		#endif
		#ifdef __APPLE__
			system("clear");
		#endif

	}
}