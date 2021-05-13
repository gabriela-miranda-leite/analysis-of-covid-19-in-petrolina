#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
/*
TRABALHO DA DISCIPLINA ALGORITMO E ESTRUTURA DE DADOS I - 2020.3 - 11/12/2020
PROFESSOR: MARCELO SANTOS LINDER
ALUNOS: GABRIELA MIRANDA DE SOUZA LEITE E VICTOR AUGUSTO MEDEIROS BALBINO
TURMA: CX
PROPOSTA: Análise do Covid-19 em Petrolina: abertura do comércio e eleição 2020
*/

//Pilha
typedef struct nodo {
    char *inf;
    struct nodo *next;
}NODO;

typedef NODO *PILHA_LINHA_ARQ;

//Fila
typedef struct 
{
	int id;
	int mes;
	int dia;
	int ac_morte;
	int ac_confirmados;
	int morte;
	int confirmados;
}DADO;

typedef struct nodo_dado 
{
	DADO dado;
	struct nodo_dado *next;
}NODO2;

typedef struct 
{
    int n_e; //tamanho da fila
    NODO2 *INICIO;
    NODO2 *FIM;
}DESCRITOR;

typedef DESCRITOR *FILA_STRUCT;

//árvore avl
struct NO{
    DADO dado;
    int altura;
    struct NO *esq;
    struct NO *dir;
};

typedef struct NO* ArvAVL;

//funções utilizadas na pilha
void criar_pilha_arquivo(PILHA_LINHA_ARQ *pp);
void inserir_pilha_arquivo(PILHA_LINHA_ARQ *pp, char *v);

void criar_pilha_arquivo(PILHA_LINHA_ARQ *pp)
{
	*pp = NULL;
}

void inserir_pilha_arquivo(PILHA_LINHA_ARQ *pp, char *v)
{
	NODO* novo;
    novo = (NODO*) malloc (sizeof(NODO));
    if(!novo)
    {
        printf("\nErro! Memória insuficiente.\n");
        exit(1);
    }
    novo->inf = v;
    novo->next = *pp;
    (*pp) = novo;
}

//função de leitura do arquivo
PILHA_LINHA_ARQ listar(char *nome_arquivo)
{		
	FILE *t;
	PILHA_LINHA_ARQ p;
	criar_pilha_arquivo(&p);
	char palavra[30];
	t = fopen(nome_arquivo, "r");
	while(1)
	{
		char* linha;
		linha = (char*) malloc(30*sizeof(char));
		fscanf(t," %[^\n]s", palavra);
		int k;
		for(k=0; palavra[k] != '\0'; k++) 
		{
			linha[k] = palavra[k];
		}
		linha[k] = '\0';
		inserir_pilha_arquivo(&p, linha);
		if(feof(t))
			break;
	}

	fclose(t);
	return p;
}

/* Tratar as linhas do arquivo que foram armazenadas na pilha*/
char* string_substring(char str[], int unsigned start, int unsigned end);
int unsigned char_at(char *string, char caracter);
DADO transformar(char *linha);

char* string_substring(char str[], int unsigned start, int unsigned end) 
{
    unsigned int i, j;
    char *sub; 
     
    if(start >= end || end > strlen(str)) {
        return NULL;
    }
     
    sub = (char *) malloc(sizeof(char) * (end - start + 1));
     
    for(i = start, j = 0; i < end; i++, j++) {
        sub[j] = str[i];
    }
     
    sub[j] = '\0';
     
    return sub;
}

int unsigned char_at(char *string, char caracter) 
{
	int i = 0;
	while(string[i++] != '\0') {
		if(string[i] == caracter) {
			return i;
		}
	}
	return -1;
}

DADO transformar(char *linha) 
{
	//10-21,5841@95#55$0 (EXEMPLO)
	//MM-DD,num de confirmados total@ num de mortes total # numero de confirmados no dia $ numero de mortes
	DADO novo;
	char *aux;

	aux = string_substring(linha,0,char_at(linha,'-'));
	sscanf(aux,"%d",&novo.mes);

	aux = string_substring(linha,char_at(linha,'-')+1,char_at(linha,','));
	sscanf(aux,"%d",&novo.dia);

	aux = string_substring(linha,char_at(linha,',')+1,char_at(linha,'@'));
	sscanf(aux,"%d",&novo.ac_confirmados);

	aux = string_substring(linha,char_at(linha,'@')+1,char_at(linha,'#'));
	sscanf(aux,"%d",&novo.ac_morte);

	aux = string_substring(linha,char_at(linha,'#')+1,char_at(linha,'$'));
	sscanf(aux,"%d",&novo.confirmados);

	aux = string_substring(linha,char_at(linha,'$')+1,strlen(linha));
	sscanf(aux,"%d",&novo.morte);

	return novo;
}

//funções utilizadas na fila
void criar_fila(FILA_STRUCT *pf);
void inserir(FILA_STRUCT f, PILHA_LINHA_ARQ p);
DADO consulta_retirar_fila(FILA_STRUCT f);

void criar_fila(FILA_STRUCT *pf)
{
    *pf = (DESCRITOR *) malloc (sizeof(DESCRITOR));
    if(!*pf)
    {
        printf("\nErro! Memória insuficiente!\n");
        exit(1);
    }

    (*pf)->INICIO = NULL;
    (*pf)->FIM = NULL;
    (*pf)->n_e = 0;
}

void inserir(FILA_STRUCT f, PILHA_LINHA_ARQ p)
{
	NODO *aux;
	for(aux = p; aux != NULL; aux = aux->next)
	{
		NODO2 *novo;
		novo = (NODO2*) malloc (sizeof(NODO2));
		if(!novo)
		{
			printf("\nErro! Memória insuficiente\n");
			exit(2);
		}
		novo->dado = transformar(aux->inf);
		novo->next = NULL;
		if(f->n_e == 0)
			f->INICIO = novo;
		else
			f->FIM->next = novo;
		f->FIM = novo;
		f->n_e++;
		novo->dado.id = f->n_e;
	}
}

DADO consulta_retirar_fila(FILA_STRUCT f)
{
    if(!f->INICIO)
    {
        printf("\nErro! Fila vazia para consulta\n");
        exit(5);
    }
    DADO v = f->INICIO->dado;
    NODO2 *aux = f->INICIO;
    f->INICIO = f->INICIO->next;
    if(!f->INICIO)
        f->FIM = NULL;
    free(aux);
    return(v);
}
		
//funções utilizadas na árvore AVL
ArvAVL* cria_ArvAVL();
int altura_NO(struct NO* no);
int fatorBalanceamento_NO(struct NO* no);
int maior(int x, int y);
void RotacaoLL(ArvAVL *A);
void RotacaoRR(ArvAVL *A);
void RotacaoLR(ArvAVL *A);
void RotacaoRL(ArvAVL *A);

ArvAVL* cria_ArvAVL()
{
    ArvAVL* raiz = (ArvAVL*) malloc(sizeof(ArvAVL));
    if(raiz != NULL)
        *raiz = NULL;
    return raiz;
}

int altura_NO(struct NO* no)
{
    if(no == NULL)
        return -1;
    else
    return no->altura;
}

int fatorBalanceamento_NO(struct NO* no)
{
    return labs(altura_NO(no->esq) - altura_NO(no->dir));
}

int maior(int x, int y)
{
    if(x > y)
        return x;
    else
        return y;
}

void RotacaoLL(ArvAVL *A)
{
    struct NO *B;
    B = (*A)->esq;
    (*A)->esq = B->dir;
    B->dir = *A;
    (*A)->altura = maior(altura_NO((*A)->esq),altura_NO((*A)->dir)) + 1;
    B->altura = maior(altura_NO(B->esq),(*A)->altura) + 1;
    *A = B;
}

void RotacaoRR(ArvAVL *A)
{
    struct NO *B;
    B = (*A)->dir;
    (*A)->dir = B->esq;
    B->esq = (*A);
    (*A)->altura = maior(altura_NO((*A)->esq),altura_NO((*A)->dir)) + 1;
    B->altura = maior(altura_NO(B->dir),(*A)->altura) + 1;
    (*A) = B;
}

void RotacaoLR(ArvAVL *A)
{
    RotacaoRR(&(*A)->esq);
    RotacaoLL(A);
}

void RotacaoRL(ArvAVL *A)
{
    RotacaoLL(&(*A)->dir);
    RotacaoRR(A);
}

/*aqui que vai fazer a inserção do nodo na arvore, que vai fazer o balanceamento quando preciso*/
/*vou inserir usando a data, essa insercao se dá pela data e mes*/
/*e possivel por com a associacao do dia e mes, gera um numero único, que e possivel ordenar de maneira crescente,
 quanto mais antigo a data*/
/*exemplo: 25/10-> 10*100+25=1025 e 11/11-> 11*100+11=1111, que é mais antigo que o anterior, podendo usar como chave para ordenação*/
int insere_ArvAVL(ArvAVL *raiz, DADO valor)
{
    int res;
    if(*raiz == NULL)
	{
        struct NO *novo;
        novo = (struct NO*)malloc(sizeof(struct NO));
        if(novo == NULL)
            return 0;

        novo->dado = valor;
        novo->altura = 0;
        novo->esq = NULL;
        novo->dir = NULL;
        *raiz = novo;
        return 1;
    }
	int data = valor.mes*100 + valor.dia;
    struct NO *atual = *raiz;
    if(data < atual->dado.mes*100+atual->dado.dia){
        if((res = insere_ArvAVL(&(atual->esq), valor)) == 1){
            if(fatorBalanceamento_NO(atual) >= 2){
                if(data < (*raiz)->esq->dado.mes*100 + (*raiz)->esq->dado.dia){
                    RotacaoLL(raiz);
                }else{
                    RotacaoLR(raiz);
                }
            }
        }
    }else{
        if(data > atual->dado.mes*100+atual->dado.dia){
            if((res = insere_ArvAVL(&(atual->dir), valor)) == 1){
                if(fatorBalanceamento_NO(atual) >= 2){
                    if((*raiz)->dir->dado.mes*100 + (*raiz)->dir->dado.dia < data){
                        RotacaoRR(raiz);
                    }else{
                        RotacaoRL(raiz);
                    }
                }
            }
        }else{
            printf("Valor duplicado!!\n");
            return 0;
        }
    }

    atual->altura = maior(altura_NO(atual->esq),altura_NO(atual->dir)) + 1;

    return res;
}

/*recebe dados da arvore, e da fila, fazendo com que os dados da fila, sejam inseridos na arvore*/
/*o for vai me ajudar a percorrer toda a fila e insira na arvore*/

void inserir_elemento(ArvAVL *t, FILA_STRUCT f)
{
	int res;
	NODO2 *aux;
	for(aux = f->INICIO; aux != NULL; aux = aux->next)
		res = insere_ArvAVL(t, aux->dado );	
}


int valida_data(int dia, int mes, int sel);
void chama_histo(ArvAVL *raiz,  int sel);
void histogram(int numbers[], int length, int n);
void comparar(ArvAVL *raiz, int sel);
char* menu_apresentacao();
void consulta_data(ArvAVL *raiz, int dia, int mes);
void consulta_data_av(ArvAVL *raiz, int dia, int mes, int quant);
int histograma_auxiliar(ArvAVL *raiz, int dia, int mes);
int retornar_id(ArvAVL *raiz, int dia, int mes);
void consulta_taxa(ArvAVL *raiz, int dia, int mes);

void histogram(int numbers[], int length, int n) 
{

	int finished = 1, i;
	for(i = 0; i < length; i++)
		if(numbers[i] - n > 0)
			finished = 0;
		
	if(finished)
		return;
	else {
		
		histogram(numbers, length, n + 1);
		
		for(i = 0; i < length; i++) {
			int number = numbers[i] - n;
			if(number > 0) 
				printf("\t   *  ");
			else
				printf("\t      ");
 		}
		 
		printf("\n");
	}
}

/*essa e usada para chamar a funcao principal, que imprime de fato os '*'*/
void chama_histo(ArvAVL *raiz,  int sel)
{
	
	int *numbers, *ids, *dia_aux,*mes_aux;
	int dia, mes, i, max;
	int i1, j, aux = 0, aux2, aux3, aux4;
  	struct NO* a = *raiz;
	
	do
	{
		printf("\n\tDigite quantas datas deseja analisar: ");
		scanf("%d", &max);
		if(max > 62)
			printf("\n\tErro! Maior que o nosso banco de dados. Digite até 62 quantidades.\n");
	}while(max > 62);
	
	printf("\n\tDigite as %d datas deseja analisar", max);
	numbers= (int*)malloc(max*sizeof(int));
	ids= (int*)malloc(max*sizeof(int));
	dia_aux= (int*)malloc(max*sizeof(int));
	mes_aux= (int*)malloc(max*sizeof(int));

	for(i = 0; i < max;i++){
		do{
		printf("\n\tData %d:\n", i+1);
		printf("\n\tDia: ");
		scanf("\n\t%d", &dia);
		printf("\n\tMês: ");
		scanf("%d", &mes); 
		if(valida_data(dia, mes, sel) == 0)
			printf("\n\tErro! Data inserida inválida ou fora do banco de dados acessado\n");
		}while(valida_data(dia, mes, sel) == 0);


		dia_aux[i] = dia;
		mes_aux[i] = mes;
		ids[i] = retornar_id(&a, dia, mes);

		numbers[i] = histograma_auxiliar(&a, dia, mes);
	}

	for(i1 = 0;i1 < max; i1++){
		for(j = 0 ;j < max; j++){
			if(ids[i1]<ids[j]){
				aux = ids[i1];
				aux2 = numbers[i1];
				aux3 = dia_aux[i1];
				aux4 = mes_aux[i1];

				ids[i1] = ids[j];
				numbers[i1] = numbers[j];
				dia_aux[i1] = dia_aux[j];
				mes_aux[i1] = mes_aux[j];

				ids[j] = aux;
				numbers[j] = aux2;
				dia_aux[j] = aux3;
				mes_aux[j] = aux4;
			}
		}
		printf("\n");
	}

	
	system("cls");
	printf("\n\n");
	printf("\tCada * equivale a dois casos confirmados no dia.\n\tCaso não apareça significa que teve menos que dois casos confirmados\n\n");
	
	histogram(numbers, max, 0);
	

	for(i = 0;i < max; i++){
		printf("\t%d/%d |", dia_aux[i], mes_aux[i]);
	}
	printf("\n\n");
	
}

/*preciso dela pois ela que vai me dizer se a data escolhida existe, ou entÃ£o estÃ¡ contida no banco de dados escolhido*/
int valida_data(int dia, int mes, int sel)
{
	if(dia < 1 || dia > 32 || mes != 5 && mes != 6 && mes != 7 && mes != 9 && mes != 10 && mes != 11 )   /*verificacao geral, se a data faz sentido*/
		return 0;
	
	if(sel == 1) /*vai entrar aqui, se a data fizer parte do banco de dados do comercio*/
	{
		if(mes != 5 && mes != 6 && mes != 7)
			return 0;
		if(mes == 6)
		{
			if(dia >= 31)
				return 0;
		}
		else if(mes == 5)
		{
			if(dia >= 32)
				return 0; 
		}
		else if(mes == 7)
		{
			if(dia > 1)
				return 0; 	
		} 
	}
	
	else if(sel == 2)
	{

		if(mes != 9 && mes != 10 && mes != 11)
			return 0;
		if(mes == 9)
		{
			if(dia < 15 || dia >= 31)
				return 0; 
		}			
		else if(mes == 10)
		{
			if(dia >= 32)
				return 0; 
		}
		else if(mes == 11)
		{
			if(dia > 15)
				return 0;
		}
	
	}

	return 1;  /*nao entrou em nenhum desses, retorno 1, que a data e válida*/
}

/*consigo fazer a comparaÃ§Ã£o de duas datas, no final chamando a funcao de consultar*/
void comparar(ArvAVL *raiz, int sel)
{
  struct NO* a = *raiz;
	int dia1, dia2, mes1, mes2;
	char op;
	printf("\n\tDigite as datas escolhidas para comparar:\n");

	printf("\n\tData 1: \n");
	do{
		printf("\n\tDia: ");
		scanf("%d", &dia1);
		printf("\n\tMês: ");
		scanf("%d", &mes1);
	
		if(valida_data(dia1, mes1, sel)==0)
				printf("\n\tData inserida inválida ou fora do banco de dados acessado!\n");

	}while(valida_data(dia1, mes1, sel)==0);

	printf("\n\n\tData 2: \n");
	do{
		printf("\n\tDia: ");
		scanf("%d", &dia2);
		printf("\n\tMês: ");
		scanf("%d", &mes2);
		while(dia1 == dia2 && mes1 == mes2)
		{	
			printf("\n\n\tErro! Data igual a primeira!\n\tTente outra data, por favor.\n\n");
			printf("\n\tDia: ");
			scanf("%d", &dia2);
			printf("\n\tMês: ");
			scanf("%d", &mes2);	
		}

	if(valida_data(dia2, mes2, sel)==0)
			printf("\n\tData inserida inválida ou fora do banco de dados acessado\n");
	}while(valida_data(dia2, mes2, sel)==0);

	system("cls");
	/*chamada para consultar indivudualmente*/
	consulta_data(&a, dia1,mes1);
	consulta_data(&a, dia2,mes2);

	do
	{
		printf("\n\tDeseja visualizar taxas sobre os períodos visualizados ? [S/N]: ");
		scanf(" %c",&op );
		if(op=='S' || op=='s')
		{
			
			consulta_taxa(&a, dia1, mes1);
			consulta_taxa(&a, dia2, mes2);
		}
		else if(!(op=='S' || op=='s') && !(op == 'N' || op == 'n'))
			printf("\n\tErro! Letra inválida!");
	}	
	while(!(op=='S' || op=='s') && !(op == 'N' || op == 'n'));
			
}

char* menu_apresentacao()
{
	system("cls");
	int op;
  	char* nome_arquivo;
	  	printf("\n\tA ideia é analisar esses dois momentos da pandemia em Petrolina.\n");
		printf("\tUm mês antes antes e depois da abertura do comércio e dois meses antes da eleição.\n");
		printf("\tE podemos analisar como o covid-19 se comportou.\n");
		printf("\n\tDeseja acessar qual banco de dados?\n");
		printf("\n\t\t1 - Analisar abertura do comércio\n");
		printf("\n\t\t2 - Analisar as eleições 2020\n");
		printf("\n\t\t3 - SAIR\n");
    while(op != 3 && op != 1 && op != 2)
    {
      printf("\n\tDigite o número: ");
      scanf("%d", &op);
      switch(op)
      {
        case 1 : nome_arquivo = "comercio.txt";
            printf("\n\tSelecionado: Abertura do comércio\n\n");
			printf("\n\n\t-----Abertura do comércio-----\n\n");
			printf("\tAs datas para análise da abertura do comércio são de 01/5 até 01/7.\n\tSendo 01/5, o dia da abertura\n\n");
			system("pause");
            break;
        case 2: nome_arquivo = "eleicoes.txt";
            printf("\n\tSelecionado: Eleições 2020\n\n");
			printf("\n\n\t---------Eleições---------\n\n");
			printf("\tAs datas para análise das eleições são de 15/9 até 15/11.\n\tSendo 15/11, o dia da eleições.\n\n");
			system("pause");
            break;
        case 3: printf("\n\tTerminado!\n");
			
            exit(0);
        default: printf("\n\tOpção inválida!\n\tTente novamente!\n");
				system("pause");
      }
    }
	return nome_arquivo;
}

/*consulta simples, mostrando um dia*/
void consulta_data(ArvAVL *raiz, int dia, int mes)
{
	int data = mes * 100 + dia; 
	int verificador = 0;
    struct NO* atual = *raiz;
    while(atual != NULL){
        if(data == (atual->dado.mes*100) + atual->dado.dia)
		{
			printf("\n\t-----------------------------------------------------------------------------------------------\n");
			printf("\t| ");
			printf("Data: %.2d/%.2d | ", atual->dado.dia, atual->dado.mes);
			printf("Confirmados: %.3d | ", atual->dado.confirmados);
			printf("Óbitos: %.2d | ", atual->dado.morte);
			printf("Confirmados totais: %.4d | ", atual->dado.ac_confirmados);
			printf("Óbitos totais: %.3d | \n", atual->dado.ac_morte);
			printf("\t-----------------------------------------------------------------------------------------------\n");
			if(atual->dado.confirmados < 0 || atual->dado.morte < 0)
				verificador = 1;
			if(verificador)
				printf("\n\t*O dado negativo, foi por conta que ocorreu correção de valores de outros dias.\n\n");
			break;
        }
        if(data > (atual->dado.mes*100) + atual->dado.dia) 
            atual = atual->dir;
        else
            atual = atual->esq;
    }
}

/*semelhante a anterior, mas esse busca uma quantidade de dias que o usuario desejar*/
void consulta_data_av(ArvAVL *raiz, int dia, int mes, int quant)
{
	int data = mes*100+dia;
	int verificador = 0;
    struct NO* atual = *raiz;
	while(quant > 0)
	{
		while(atual != NULL){
			if(data == (atual->dado.mes*100)+atual->dado.dia)
			{
				printf("\n\t-----------------------------------------------------------------------------------------------\n");
				printf("\t| ");
				printf("Data: %.2d/%.2d | ", atual->dado.dia, atual->dado.mes);
				printf("Confirmados: %.3d | ", atual->dado.confirmados);
				printf("Óbitos: %.2d | ", atual->dado.morte);
				printf("Confirmados totais: %.4d | ", atual->dado.ac_confirmados);
				printf("Óbitos totais: %.3d | \n", atual->dado.ac_morte);
				printf("\t-----------------------------------------------------------------------------------------------\n");
				if(atual->dado.confirmados < 0 || atual->dado.morte < 0)
					verificador = 1;
				if(dia == 30 && mes == 9)
				{
					dia = 0;
					mes = 10;
				}
				if(dia == 31 && mes == 10)
				{
					dia = 0;
					mes = 11;
				}
				if(dia == 15 && mes == 11)
				{		
					printf("\n\tAtenção! Quantidade limite atingida!\n");
					quant = 1;
				}
				if(dia == 30 && mes == 6)
				{
					dia = 0;
					mes = 7;
					quant = 2;
				}
				if(dia == 31 && mes == 5)
				{
					dia = 0;
					mes = 6;
				}	
				if(dia == 1 && mes == 7)
				{		
					printf("\n\tAtenção! Quantidade limite atingida!\n");
					quant = 1;
				}
				dia++;
				quant--;
				consulta_data_av(raiz, dia, mes, quant);
			}
			if(data > (atual->dado.mes*100)+atual->dado.dia)
				atual = atual->dir;
			else
				atual = atual->esq;
		}
		break;
	}
}

/*essa funcao e usada pela funcao de comparar*/
void consulta_taxa(ArvAVL *raiz, int dia, int mes)
{
	int data = mes*100+dia; 
	float  taxa_mort=0, taxa_let=0;
    struct NO* atual = *raiz;
    while(atual != NULL){
        if(data == (atual->dado.mes*100)+atual->dado.dia)
		{
			taxa_mort=(float)atual->dado.ac_morte/(float)354317;
			taxa_let=(float)atual->dado.ac_morte/(float)atual->dado.ac_confirmados;
			printf("\n\t-------------------------------------------------------------------------\n");
			printf("\t| Data: %d/%d |", dia, mes);
			printf("Taxa de mortalidade: %3f %% |", taxa_mort*100);	 
			printf("Taxa de letalidade: %3f %% |\n", taxa_let*100);	
			printf("\t-------------------------------------------------------------------------\n");
			break;
        }
        if(data > (atual->dado.mes*100)+atual->dado.dia)
            atual = atual->dir;
        else
            atual = atual->esq;
    }
}

/*essa funcao e usada pra poder saber a quantidade de * que vai pra funcao q imprime*/
int histograma_auxiliar(ArvAVL *raiz, int dia, int mes)
{
	int data = mes*100+dia; 
    struct NO* atual = *raiz;
    while(atual != NULL){
        if(data == (atual->dado.mes*100)+atual->dado.dia)
		{
			return((atual->dado.confirmados)/2);
			break;
        }
        if(data > (atual->dado.mes*100)+atual->dado.dia)
            atual = atual->dir;
        else
            atual = atual->esq;
    }
    return -1;
}

int retornar_id(ArvAVL *raiz, int dia, int mes)
{
	int data = mes*100+dia; 
    struct NO* atual = *raiz;
    while(atual != NULL){
        if(data == (atual->dado.mes*100)+atual->dado.dia)
		{
			return((atual->dado.id));
			break;
        }
        if(data > (atual->dado.mes*100)+atual->dado.dia)
            atual = atual->dir;
        else
            atual = atual->esq;
    }
    return -1;
}

int main()
{
	setlocale(LC_ALL, "portuguese" );
	
	char *nome_arquivo;
	FILA_STRUCT f;
	ArvAVL *a;
	int op, res;
	criar_fila(&f);
	nome_arquivo = menu_apresentacao();
	inserir(f, listar(nome_arquivo));
 	a = cria_ArvAVL();
	inserir_elemento(a, f);
  	int op2 = 1;

	while(op2 != 6)
	{
		system("cls");
		if(strcmp (nome_arquivo, "comercio.txt") == 0)
			op = 1;
		else 
			op = 2;

		printf("\n\tDeseja acessar:\n");
		printf("\n\t\t1 - Uma data específica\n");
		printf("\n\t\t2 - Comparar duas datas\n");
		printf("\n\t\t3 - Um intervalo de datas\n");
		printf("\n\t\t4 - Gráfico\n");
		printf("\n\t\t5 - Mudar banco de dados\n");
		printf("\n\t\t6 - SAIR\n");
		printf("\n\tDigite o número: ");
		scanf("%d", &op2);
		switch(op2)
		{
		case 1: 
				system("cls");
				printf("\n\t-> Uma data específica\n");
				int d1, m1;
				do
				{
					printf("\n\tDigite  dia: ");
					scanf("%d", &d1);
					printf("\n\tDigite mês: ");
					scanf("%d", &m1);
					
					if(valida_data(d1, m1, op))
					{
						consulta_data(a, d1, m1);
						system("pause");
					}
					else
					{
						printf("\nErro! Data inválida!\n");
						system("pause");
					}
				}while(valida_data(d1, m1, op) == 0);
				break;

		case 2: 
				system("cls");
				printf("\n\t-> Comparar duas datas\n");
				comparar(a, op);
				system("pause");
				break;
				
		case 3:
				system("cls");
				printf("\n\t-> Um intervalo de datas\n");
				int qtd;
				do
				{
					printf("\n\tDigite  dia: ");
					scanf("%d", &d1);
					printf("\n\tDigite mês: ");
					scanf("%d", &m1);
					printf("\n\tQuantas datas você deseja acessar a partir do dia %d/%d.\n\tDigite: ", d1, m1);
					scanf("%d", &qtd);
					if(valida_data(d1, m1, op) == 1 && qtd > 0)
						consulta_data_av(a, d1, m1, qtd);
					else
						if(qtd < 1 && valida_data(d1, m1, op) == 1)
						{
							printf("\n\tErro! Quantidade inválida. Menor que 1!\n");
							system("pause");
						}
						else if(qtd > 0 && valida_data(d1, m1, op) == 0)
						{
							printf("\n\tErro! Data inválida.\n");
							system("pause");
						}
						else 
						{
							printf("\n\tErro! Data inválida e quantidade inválida (menor que 1).\n");
							system("pause");
						}
						
				}while(valida_data(d1, m1, op) == 0 || qtd < 1);
				system("pause");
				break;

		case 4: 
				system("cls");
				printf("\n\t-> Gráfico\n");
				chama_histo(a, op);
				system("pause");
				break;

		case 5: 
				system("cls");
				printf("\n\t-> Mudar bando de dados!\n");
				menu_apresentacao();
				nome_arquivo = menu_apresentacao();
				inserir(f, listar(nome_arquivo));
				a = cria_ArvAVL();
				inserir_elemento(a, f);
				break;

		case 6: 
				printf("\n\t-> Programa finalizado!\n");
            	exit(0);
		
		default: 
			system("cls");
			printf("\n\tOpção inválida!\n\tTente novamente!\n");
			system("pause");	
	}

}
	
	return 0;
}


